// 教程 4：ESP32-C3 + MPU-6050 + 8x8 WS2812 重力感应沙漏
// 需要先在 Arduino IDE 的库管理器中安装：
// 1. Adafruit NeoPixel
// 2. Adafruit MPU6050
// 3. Adafruit Unified Sensor（安装 Adafruit MPU6050 时通常会自动安装）

#include <Adafruit_MPU6050.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

const int WS2812_PIN = 2;
const int SDA_PIN = 4;
const int SCL_PIN = 5;

const int MATRIX_WIDTH = 8;
const int MATRIX_HEIGHT = 8;
const int LED_COUNT = MATRIX_WIDTH * MATRIX_HEIGHT;
const int BRIGHTNESS = 28;

const int GRAIN_COUNT = 22;
const unsigned long FRAME_INTERVAL_MS = 70;
const float MIN_GRAVITY_COMPONENT = 0.25;

// 如果你发现沙子方向和实际倾斜方向相反，可以把下面的 1 改成 -1。
const int ACCEL_X_SIGN = 1;
const int ACCEL_Y_SIGN = 1;

struct Grain {
  int x;
  int y;
};

Adafruit_NeoPixel matrix(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MPU6050 mpu;
Grain grains[GRAIN_COUNT];
bool occupied[MATRIX_WIDTH][MATRIX_HEIGHT];
unsigned long lastFrameAt = 0;

int xyToPixel(int x, int y) {
  if (y % 2 == 0) {
    return y * MATRIX_WIDTH + x;
  }

  return y * MATRIX_WIDTH + (MATRIX_WIDTH - 1 - x);
}

bool insideMatrix(int x, int y) {
  return x >= 0 && x < MATRIX_WIDTH && y >= 0 && y < MATRIX_HEIGHT;
}

bool isSandCell(int x, int y) {
  if (!insideMatrix(x, y)) {
    return false;
  }

  // 8x8 空间很小，这里把可活动区域做成沙漏形：
  // 上半部分逐行收窄，中间保留 2 列“漏口”，下半部分再逐行变宽。
  if (y <= 2) {
    return x > y && x < MATRIX_WIDTH - 1 - y;
  }

  if (y <= 5) {
    return x == 3 || x == 4;
  }

  return x < y && x > MATRIX_WIDTH - 1 - y;
}

void clearOccupied() {
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = 0; y < MATRIX_HEIGHT; y++) {
      occupied[x][y] = false;
    }
  }
}

void rebuildOccupied() {
  clearOccupied();
  for (int i = 0; i < GRAIN_COUNT; i++) {
    occupied[grains[i].x][grains[i].y] = true;
  }
}

void seedSand() {
  int index = 0;
  for (int y = 0; y < MATRIX_HEIGHT && index < GRAIN_COUNT; y++) {
    for (int x = 0; x < MATRIX_WIDTH && index < GRAIN_COUNT; x++) {
      if (isSandCell(x, y)) {
        grains[index] = {x, y};
        index++;
      }
    }
  }
  rebuildOccupied();
}

int componentToStep(float value) {
  if (value > MIN_GRAVITY_COMPONENT) {
    return 1;
  }

  if (value < -MIN_GRAVITY_COMPONENT) {
    return -1;
  }

  return 0;
}

bool tryMoveGrain(int grainIndex, int nextX, int nextY) {
  if (!isSandCell(nextX, nextY) || occupied[nextX][nextY]) {
    return false;
  }

  occupied[grains[grainIndex].x][grains[grainIndex].y] = false;
  grains[grainIndex].x = nextX;
  grains[grainIndex].y = nextY;
  occupied[nextX][nextY] = true;
  return true;
}

void moveOneGrain(int grainIndex, int gravityX, int gravityY) {
  int currentX = grains[grainIndex].x;
  int currentY = grains[grainIndex].y;

  if (gravityX == 0 && gravityY == 0) {
    return;
  }

  if (tryMoveGrain(grainIndex, currentX + gravityX, currentY + gravityY)) {
    return;
  }

  if (gravityX != 0 && tryMoveGrain(grainIndex, currentX + gravityX, currentY)) {
    return;
  }

  if (gravityY != 0 && tryMoveGrain(grainIndex, currentX, currentY + gravityY)) {
    return;
  }

  if (gravityX != 0 && gravityY != 0) {
    if (random(2) == 0) {
      tryMoveGrain(grainIndex, currentX - gravityX, currentY + gravityY);
    } else {
      tryMoveGrain(grainIndex, currentX + gravityX, currentY - gravityY);
    }
  }
}

void updateSand(float accelX, float accelY) {
  int gravityX = componentToStep(accelX);
  int gravityY = componentToStep(accelY);

  if (fabs(accelX) > fabs(accelY) * 1.4) {
    gravityY = 0;
  } else if (fabs(accelY) > fabs(accelX) * 1.4) {
    gravityX = 0;
  }

  int start = gravityY > 0 ? GRAIN_COUNT - 1 : 0;
  int end = gravityY > 0 ? -1 : GRAIN_COUNT;
  int step = gravityY > 0 ? -1 : 1;

  for (int i = start; i != end; i += step) {
    int grainIndex = (i + random(GRAIN_COUNT)) % GRAIN_COUNT;
    moveOneGrain(grainIndex, gravityX, gravityY);
  }
}

void renderHourglass() {
  matrix.clear();

  for (int y = 0; y < MATRIX_HEIGHT; y++) {
    for (int x = 0; x < MATRIX_WIDTH; x++) {
      if (!isSandCell(x, y)) {
        matrix.setPixelColor(xyToPixel(x, y), matrix.Color(8, 5, 1));
      }
    }
  }

  for (int i = 0; i < GRAIN_COUNT; i++) {
    matrix.setPixelColor(xyToPixel(grains[i].x, grains[i].y), matrix.Color(255, 120, 0));
  }

  matrix.show();
}

void setup() {
  Serial.begin(115200);
  delay(200);

  randomSeed(esp_random());
  Wire.begin(SDA_PIN, SCL_PIN);

  matrix.begin();
  matrix.setBrightness(BRIGHTNESS);
  matrix.clear();
  matrix.show();

  if (!mpu.begin()) {
    Serial.println("没有找到 MPU-6050，请检查 SDA/SCL、VCC、GND 接线。");
    while (true) {
      matrix.fill(matrix.Color(80, 0, 0));
      matrix.show();
      delay(250);
      matrix.clear();
      matrix.show();
      delay(250);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  seedSand();
  renderHourglass();
}

void loop() {
  if (millis() - lastFrameAt < FRAME_INTERVAL_MS) {
    return;
  }
  lastFrameAt = millis();

  sensors_event_t acceleration;
  sensors_event_t gyro;
  sensors_event_t temperature;
  mpu.getEvent(&acceleration, &gyro, &temperature);

  float accelX = acceleration.acceleration.x * ACCEL_X_SIGN;
  float accelY = acceleration.acceleration.y * ACCEL_Y_SIGN;

  updateSand(accelX, accelY);
  renderHourglass();
}
