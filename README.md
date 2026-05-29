# ESP32-C3 Arduino 教程仓库

这个仓库用于存放基于 **Arduino** 的 ESP32-C3 入门教程。课程不需要使用 ESP-IDF，所有示例都可以在 Arduino IDE 或 Arduino CLI 中打开、编译和上传。

## 环境准备

1. 安装 Arduino IDE 2.x。
2. 打开 **Preferences / 首选项**，在 **Additional boards manager URLs / 附加开发板管理器网址** 中加入：

   ```text
   https://espressif.github.io/arduino-esp32/package_esp32_index.json
   ```

3. 打开 **Boards Manager / 开发板管理器**，搜索并安装 **esp32 by Espressif Systems**。
4. 选择你的 ESP32-C3 开发板，例如 **ESP32C3 Dev Module**。
5. 连接开发板，选择正确串口后即可上传示例。

## 教程目录

| 序号 | 教程 | 控制引脚 | 说明 |
| --- | --- | --- | --- |
| 1 | [ESP32-C3 点亮普通 LED](tutorials/01_esp32c3_blink_led/README.md) | GPIO2 | 使用 `digitalWrite()` 控制 LED 闪烁 |
| 2 | [ESP32-C3 点亮 WS2812](tutorials/02_esp32c3_ws2812/README.md) | GPIO2 | 使用 Adafruit NeoPixel 库控制 WS2812 彩灯 |
| 4 | [ESP32-C3 + MPU-6050 制作 8x8 WS2812 重力感应沙漏](tutorials/04_esp32c3_mpu6050_ws2812_hourglass/README.md) | GPIO2 / GPIO4 / GPIO5 | 读取 MPU-6050 加速度数据，让 8x8 WS2812 点阵显示重力感应沙漏 |

## Arduino 草图命名说明

Arduino IDE 要求 `.ino` 文件所在文件夹与 `.ino` 文件同名，所以每个教程目录下会再放一个同名草图文件夹。例如：

```text
tutorials/01_esp32c3_blink_led/esp32c3_blink_led/esp32c3_blink_led.ino
```

打开教程时，请直接打开对应的 `.ino` 文件。
