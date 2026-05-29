# ESP32-C3 Blink LED：GPIO2 闪烁

本教程演示如何使用 ESP-IDF GPIO API 控制 ESP32-C3 的 GPIO2 输出高低电平，实现 LED 闪烁。

## 示例信息

- 目标芯片：ESP32-C3
- 控制引脚：GPIO2
- 闪烁间隔：500ms

## 环境准备

请先安装并配置 ESP-IDF 开发环境，然后在本教程目录下执行以下命令。

## 设置目标芯片

```bash
idf.py set-target esp32c3
```

## 编译

```bash
idf.py build
```

## 烧录与监视串口输出

将开发板连接到电脑后，把 `<PORT>` 替换为实际串口设备，例如 Linux/macOS 下的 `/dev/ttyUSB0` 或 Windows 下的 `COM3`。

```bash
idf.py -p <PORT> flash monitor
```

## 示例行为

程序启动后会配置 GPIO2 为输出模式，并在 `app_main()` 中循环拉高和拉低 GPIO2，每次状态切换间隔 500ms。
