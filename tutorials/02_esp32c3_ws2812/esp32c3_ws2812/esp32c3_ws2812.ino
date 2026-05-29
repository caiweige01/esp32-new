// 教程 2：ESP32-C3 使用 GPIO2 点亮 WS2812
// 需要先在 Arduino IDE 的库管理器中安装 Adafruit NeoPixel。

#include <Adafruit_NeoPixel.h>

const int WS2812_PIN = 2;
const int LED_COUNT = 1;
const int BRIGHTNESS = 40;
const unsigned long COLOR_DELAY_MS = 1000;

Adafruit_NeoPixel strip(LED_COUNT, WS2812_PIN, NEO_GRB + NEO_KHZ800);

void showColor(uint8_t red, uint8_t green, uint8_t blue) {
  strip.clear();
  strip.setPixelColor(0, strip.Color(red, green, blue));
  strip.show();
}

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  showColor(255, 0, 0);   // 红色
  delay(COLOR_DELAY_MS);

  showColor(0, 255, 0);   // 绿色
  delay(COLOR_DELAY_MS);

  showColor(0, 0, 255);   // 蓝色
  delay(COLOR_DELAY_MS);
}
