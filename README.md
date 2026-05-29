# ESP32-C3 Arduino 入门教程

用 Arduino IDE 快速上手 ESP32-C3：从普通 LED，到 WS2812 彩灯，再到 MPU-6050 重力感应沙漏。

## 快速开始

1. 安装 **Arduino IDE 2.x**。
2. 在 **首选项 / Preferences** 的 **附加开发板管理器网址** 中加入：

   ```text
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```

3. 在 **开发板管理器 / Boards Manager** 中安装 **esp32 by Espressif Systems**。
4. 选择开发板（例如 **ESP32C3 Dev Module**）和正确串口。
5. 打开教程中的 `.ino` 文件，点击上传。

## 教程列表

| # | 教程 | 用到的模块 | 引脚 |
| --- | --- | --- | --- |
| 1 | [点亮普通 LED](tutorials/01_esp32c3_blink_led/README.md) | LED | GPIO2 |
| 2 | [点亮 WS2812 彩灯](tutorials/02_esp32c3_ws2812/README.md) | WS2812 / NeoPixel | GPIO2 |
| 3 | [制作 8x8 WS2812 重力感应沙漏](tutorials/03_esp32c3_mpu6050_ws2812_hourglass/README.md) | WS2812 点阵、MPU-6050 | GPIO2、GPIO4、GPIO5 |

> 原目录中从教程 2 直接跳到教程 4，确实遗漏了“第 3 个教程”的编号。当前已把沙漏项目整理为教程 3，教程列表保持连续。

## 打开草图

Arduino IDE 要求 `.ino` 文件所在文件夹与 `.ino` 文件同名。请进入对应教程目录，打开里面的 `.ino` 文件，例如：

```text
tutorials/01_esp32c3_blink_led/esp32c3_blink_led/esp32c3_blink_led.ino
```
