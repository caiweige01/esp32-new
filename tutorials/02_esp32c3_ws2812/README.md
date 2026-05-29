# 02 ESP32-C3 WS2812 RGB LED 示例

本教程演示如何在 ESP32-C3 上使用 ESP-IDF 的 `led_strip` 组件和 RMT 驱动控制 WS2812 RGB LED，并循环显示红、绿、蓝三种颜色。

## 硬件说明

- 目标芯片：ESP32-C3
- WS2812 数据引脚：GPIO2
- LED 数量：示例默认初始化 1 颗 WS2812 LED，可在 `main/main.c` 中修改 `WS2812_LED_COUNT` 扩展为更多 LED。

## 接线说明

| WS2812 引脚 | ESP32-C3 / 电源连接 |
| --- | --- |
| DIN / DI | GPIO2 |
| VCC / 5V / 3V3 | 稳定的 5V 或 3.3V 电源 |
| GND | ESP32-C3 GND 与电源 GND 共地 |

> 供电注意事项：WS2812 通常需要稳定 5V/3.3V 供电，GND 必须与 ESP32-C3 共地。多个 LED 同时点亮时电流会明显增加，请根据 LED 数量选择足够电流能力的电源，不建议直接从开发板 3.3V 引脚给大量 LED 供电。

## 软件说明

示例使用 ESP-IDF 组件管理器拉取 `espressif/led_strip` 组件。核心逻辑位于 `main/main.c`：

1. 将 WS2812 数据引脚配置为 GPIO2。
2. 使用 RMT 后端初始化 `led_strip`。
3. 循环设置第 1 颗 LED 为红、绿、蓝三种颜色。

## 编译、烧录、监视串口

进入示例目录：

```bash
cd tutorials/02_esp32c3_ws2812
```

设置目标芯片：

```bash
idf.py set-target esp32c3
```

编译工程：

```bash
idf.py build
```

烧录到开发板（请将 `/dev/ttyUSB0` 替换为你的串口设备）：

```bash
idf.py -p /dev/ttyUSB0 flash
```

监视串口输出：

```bash
idf.py -p /dev/ttyUSB0 monitor
```

也可以一次完成编译、烧录和监视：

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

在串口监视器中按 `Ctrl+]` 退出。
