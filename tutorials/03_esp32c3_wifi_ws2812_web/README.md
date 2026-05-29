# 教程 3：WiFi 网页控制 WS2812 灯光效果

本教程演示如何让 ESP32-C3 开启一个 WiFi 热点，并在浏览器页面中控制 **GPIO2** 上的 WS2812 灯光效果。灯光库使用 **FastLED**。

完成后，你可以用手机或电脑连接 ESP32-C3 的热点，打开网页设置颜色、亮度、速度和动画模式。


## 仓库位置

本教程已经整理在仓库的独立目录中：

```text
tutorials/03_esp32c3_wifi_ws2812_web/
├── README.md
└── esp32c3_wifi_ws2812_web/
    └── esp32c3_wifi_ws2812_web.ino
```

Arduino IDE 要求 `.ino` 文件所在文件夹与 `.ino` 文件同名，因此请打开上面子目录中的 `esp32c3_wifi_ws2812_web.ino`，不要只打开外层教程目录。

## 硬件准备

- ESP32-C3 开发板 × 1
- WS2812 / NeoPixel 灯珠、灯环或灯带 × 1
- 外部电源（灯珠数量较多时推荐）
- 面包板和杜邦线若干

## 安装 Arduino 库

1. 打开 Arduino IDE。
2. 进入 **Library Manager / 库管理器**。
3. 搜索 **FastLED**。
4. 安装 **FastLED by Daniel Garcia**。

ESP32-C3 的 WiFi 和 WebServer 功能来自 Arduino ESP32 开发板包，不需要额外安装库。

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
tutorials/03_esp32c3_wifi_ws2812_web/esp32c3_wifi_ws2812_web/esp32c3_wifi_ws2812_web.ino
```

## 上传前需要确认

在代码顶部可以看到这些配置：

```cpp
const char* AP_SSID = "ESP32C3-WS2812";
const char* AP_PASSWORD = "12345678";

const uint8_t WS2812_PIN = 2;
const uint16_t LED_COUNT = 8;
```

- `AP_SSID`：ESP32-C3 创建的 WiFi 热点名称。
- `AP_PASSWORD`：热点密码，至少 8 位。
- `WS2812_PIN`：WS2812 数据引脚，本教程使用 GPIO2。
- `LED_COUNT`：灯珠数量，请按你的灯带、灯环或灯板实际数量修改。

## 上传步骤

1. 在 Arduino IDE 中选择开发板，例如 **ESP32C3 Dev Module**。
2. 选择正确的串口。
3. 确认已经安装 **FastLED** 库。
4. 点击 **Upload / 上传**。
5. 上传完成后，打开串口监视器，波特率选择 `115200`。
6. 看到提示后，用手机或电脑连接 WiFi 热点 `ESP32C3-WS2812`，密码是 `12345678`。
7. 在浏览器打开：

```text
http://192.168.4.1
```

## 页面功能

网页界面保持简洁，包含 4 个主要控制项：

- **灯光效果**：支持彩虹流动、固定颜色、呼吸灯、追逐灯、关闭。
- **颜色**：选择固定颜色、呼吸灯和追逐灯使用的颜色。
- **亮度**：范围 `0 ~ 255`。
- **速度**：范围 `10 ~ 200` 毫秒，数值越小动画越快。

点击 **应用到灯带** 后，浏览器会通过 `/api/set` 接口把设置发送给 ESP32-C3。

## 代码重点

- `WiFi.softAP(AP_SSID, AP_PASSWORD);`：让 ESP32-C3 开启自己的 WiFi 热点。
- `WebServer server(80);`：启动 HTTP 服务，浏览器通过 80 端口访问网页。
- `server.on("/", handleRoot);`：返回控制页面。
- `server.on("/api/set", handleSet);`：接收网页发来的灯光设置。
- `FastLED.addLeds<WS2812B, WS2812_PIN, GRB>(...)`：指定 WS2812 类型、GPIO2 和 GRB 色彩顺序。
- `updateLights()`：在主循环中非阻塞更新灯光动画，同时保持网页响应。
