# 教程 2：使用 ESP32-C3 点亮 WS2812

本教程演示如何使用 Arduino 和 **Adafruit NeoPixel** 库，通过 ESP32-C3 的 **GPIO2** 控制 WS2812 彩灯。

## 硬件准备

- ESP32-C3 开发板 × 1
- WS2812 / NeoPixel 灯珠、灯环或灯带 × 1
- 外部电源（灯珠数量较多时推荐）
- 面包板和杜邦线若干

## 安装 Arduino 库

1. 打开 Arduino IDE。
2. 进入 **Library Manager / 库管理器**。
3. 搜索 **Adafruit NeoPixel**。
4. 安装 **Adafruit NeoPixel by Adafruit**。

## 接线方式

| ESP32-C3 / 电源 | WS2812 |
| --- | --- |
| GPIO2 | DIN / Data In |
| 3V3 或外部 5V | VCC / 5V |
| GND | GND |

> 注意：如果 WS2812 使用外部电源供电，外部电源 GND 必须和 ESP32-C3 的 GND 共地。灯珠数量较多时，不要直接从开发板 3V3 引脚取电。

## 打开示例

用 Arduino IDE 打开下面这个草图文件：

```text
tutorials/02_esp32c3_ws2812/esp32c3_ws2812/esp32c3_ws2812.ino
```

## 上传步骤

1. 在 Arduino IDE 中选择开发板，例如 **ESP32C3 Dev Module**。
2. 选择正确的串口。
3. 确认已经安装 **Adafruit NeoPixel** 库。
4. 点击 **Upload / 上传**。
5. 上传完成后，WS2812 会依次显示红、绿、蓝三种颜色。

## 代码重点

- `const int WS2812_PIN = 2;`：指定 WS2812 数据引脚为 GPIO2。
- `Adafruit_NeoPixel strip(...)`：创建 WS2812 控制对象。
- `strip.setPixelColor()`：设置灯珠颜色。
- `strip.show()`：把颜色数据发送到 WS2812。
