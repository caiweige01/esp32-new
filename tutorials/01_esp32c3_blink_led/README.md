# 教程 1：使用 ESP32-C3 点亮普通 LED

本教程演示如何使用 Arduino 的 `digitalWrite()` 函数，通过 ESP32-C3 的 **GPIO2** 控制一个普通 LED 闪烁。

## 硬件准备

- ESP32-C3 开发板 × 1
- LED × 1
- 220Ω 到 1kΩ 电阻 × 1
- 面包板和杜邦线若干

## 接线方式

| ESP32-C3 | 元件 |
| --- | --- |
| GPIO2 | 电阻一端 |
| 电阻另一端 | LED 正极（长脚） |
| GND | LED 负极（短脚） |

如果你的开发板已经在 GPIO2 接了板载 LED，可以先不外接 LED，直接上传程序观察板载 LED。

## 打开示例

用 Arduino IDE 打开下面这个草图文件：

```text
tutorials/01_esp32c3_blink_led/esp32c3_blink_led/esp32c3_blink_led.ino
```

## 上传步骤

1. 在 Arduino IDE 中选择开发板，例如 **ESP32C3 Dev Module**。
2. 选择正确的串口。
3. 点击 **Upload / 上传**。
4. 上传完成后，LED 会每 500 毫秒切换一次亮灭状态。

## 代码重点

- `const int LED_PIN = 2;`：指定 LED 控制引脚为 GPIO2。
- `pinMode(LED_PIN, OUTPUT);`：把 GPIO2 设置为输出模式。
- `digitalWrite()`：输出高低电平，让 LED 亮灭。
