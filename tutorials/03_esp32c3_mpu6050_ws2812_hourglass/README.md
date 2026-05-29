# 教程 3：ESP32-C3 + MPU-6050 制作 8x8 WS2812 重力感应沙漏

本教程演示如何使用 **ESP32-C3** 读取 **MPU-6050** 的加速度数据，并在一块 **8x8 WS2812 / NeoPixel 点阵** 上做一个会随着重力方向流动的电子沙漏。

> MPU-6050 同时包含三轴加速度计和三轴陀螺仪。本项目判断“重力方向”主要使用加速度计数据，陀螺仪会由库初始化但不直接参与沙粒运动计算。

## 硬件准备

- ESP32-C3 开发板 × 1
- MPU-6050 模块 × 1
- 8x8 WS2812 / NeoPixel 点阵 × 1
- 5V 外部电源 × 1（推荐，点阵全亮电流较大）
- 面包板和杜邦线若干

## 安装 Arduino 库

1. 打开 Arduino IDE。
2. 进入 **Library Manager / 库管理器**。
3. 搜索并安装 **Adafruit NeoPixel by Adafruit**。
4. 搜索并安装 **Adafruit MPU6050 by Adafruit**。
5. 如果 Arduino IDE 提示安装依赖，请同时安装 **Adafruit Unified Sensor** 和 **Adafruit BusIO**。

## 接线方式

| ESP32-C3 / 电源 | 8x8 WS2812 点阵 |
| --- | --- |
| GPIO2 | DIN / Data In |
| 外部 5V | 5V / VCC |
| GND | GND |

| ESP32-C3 | MPU-6050 |
| --- | --- |
| GPIO4 | SDA |
| GPIO5 | SCL |
| 3V3 | VCC |
| GND | GND |

> 注意：如果 WS2812 使用外部 5V 电源供电，外部电源 GND 必须和 ESP32-C3 的 GND 共地。不要让 8x8 点阵直接从开发板 3V3 引脚取电。

## 安装方向

为了让沙子能根据重力流动，请把 **MPU-6050 模块和 8x8 点阵固定在同一个竖直平面上**。如果发现沙子方向和实际倾斜方向相反，可以打开草图，把下面两个常量中的 `1` 改成 `-1` 进行校准：

```cpp
const int ACCEL_X_SIGN = 1;
const int ACCEL_Y_SIGN = 1;
```

## 打开示例

用 Arduino IDE 打开下面这个草图文件：

```text
tutorials/03_esp32c3_mpu6050_ws2812_hourglass/esp32c3_mpu6050_ws2812_hourglass/esp32c3_mpu6050_ws2812_hourglass.ino
```

## 上传步骤

1. 在 Arduino IDE 中选择开发板，例如 **ESP32C3 Dev Module**。
2. 选择正确的串口。
3. 确认已经安装 **Adafruit NeoPixel**、**Adafruit MPU6050** 及其依赖库。
4. 点击 **Upload / 上传**。
5. 上传完成后，8x8 点阵会显示一个暗色沙漏轮廓，橙色沙粒会随倾斜方向移动。

## 代码重点

- `Wire.begin(SDA_PIN, SCL_PIN);`：指定 ESP32-C3 的 I2C 引脚，用来读取 MPU-6050。
- `mpu.getEvent(...)`：读取 MPU-6050 当前的加速度、角速度和温度数据。
- `updateSand(accelX, accelY);`：把加速度方向转换成沙粒移动方向。
- `xyToPixel(x, y)`：把 8x8 坐标转换成 WS2812 点阵的一维灯珠序号，默认适配蛇形布线点阵。
- `renderHourglass();`：绘制沙漏轮廓和沙粒。
