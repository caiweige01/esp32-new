# ESP32-C3 教程索引

本目录按学习顺序整理所有 Arduino 版 ESP32-C3 教程。每个教程目录都包含：

- `README.md`：硬件准备、接线、库安装、上传步骤和代码重点。
- 与 Arduino 草图同名的子目录：满足 Arduino IDE 对 `.ino` 文件夹命名的要求。
- `.ino` 示例：可直接用 Arduino IDE 或 Arduino CLI 打开、编译和上传。

## 推荐学习顺序

| 序号 | 教程目录 | 主要内容 | 需要额外 Arduino 库 |
| --- | --- | --- | --- |
| 1 | [`01_esp32c3_blink_led`](01_esp32c3_blink_led/README.md) | 用 GPIO2 控制普通 LED 闪烁，熟悉上传流程 | 无 |
| 2 | [`02_esp32c3_ws2812`](02_esp32c3_ws2812/README.md) | 用 Adafruit NeoPixel 控制 WS2812 显示红绿蓝 | Adafruit NeoPixel |
| 4 | [`04_esp32c3_wifi_ws2812_web`](04_esp32c3_wifi_ws2812_web/README.md) | ESP32-C3 开 WiFi 热点，用网页控制 WS2812 动画 | FastLED |

## 通用硬件约定

- 默认控制引脚均使用 **GPIO2**，如果你的开发板 GPIO2 被占用，请同步修改教程 README 和 `.ino` 文件中的引脚常量。
- 使用 WS2812 灯带、灯环或灯板时，灯珠较多建议使用外部 5V 电源。
- 外部电源供电时，外部电源 **GND 必须与 ESP32-C3 GND 共地**。
- 上传前确认 Arduino IDE 已选择 ESP32-C3 对应开发板和正确串口。

## 使用 Arduino CLI 的可选方式

如果你偏好命令行，也可以在仓库根目录使用 Arduino CLI 编译示例。下面以教程 4 为例：

```bash
arduino-cli compile \
  --fqbn esp32:esp32:esp32c3 \
  tutorials/04_esp32c3_wifi_ws2812_web/esp32c3_wifi_ws2812_web
```

不同开发板的 FQBN 可能不同，请用 `arduino-cli board listall esp32` 查询后替换。
