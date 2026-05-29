// 教程 4：通过 WiFi 网页控制 ESP32-C3 上的 WS2812 灯光效果
// 需要先在 Arduino IDE 的库管理器中安装 FastLED。

#include <WiFi.h>
#include <WebServer.h>
#include <FastLED.h>

const char* AP_SSID = "ESP32C3-WS2812";
const char* AP_PASSWORD = "12345678";

const uint8_t WS2812_PIN = 2;
const uint16_t LED_COUNT = 8;  // 按你的灯珠数量修改
const uint8_t DEFAULT_BRIGHTNESS = 80;
const uint16_t DEFAULT_SPEED_MS = 35;

CRGB leds[LED_COUNT];
WebServer server(80);

String currentMode = "rainbow";
CRGB currentColor = CRGB(255, 80, 0);
uint8_t currentBrightness = DEFAULT_BRIGHTNESS;
uint16_t currentSpeedMs = DEFAULT_SPEED_MS;
uint8_t animationStep = 0;
unsigned long lastFrameMs = 0;

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>ESP32-C3 WS2812 控制台</title>
  <style>
    :root { color-scheme: dark; font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif; }
    body { margin: 0; min-height: 100vh; display: grid; place-items: center; background: radial-gradient(circle at top, #27314f, #10131f 58%); color: #f8fbff; }
    main { width: min(92vw, 430px); padding: 24px; border: 1px solid rgba(255,255,255,.12); border-radius: 24px; background: rgba(15,19,32,.78); box-shadow: 0 24px 80px rgba(0,0,0,.35); }
    h1 { margin: 0 0 6px; font-size: 26px; }
    p { margin: 0 0 22px; color: #aeb8d4; }
    label { display: block; margin: 18px 0 8px; font-size: 14px; color: #cad4ef; }
    select, input[type="range"], input[type="color"] { width: 100%; }
    select { padding: 12px; border: 0; border-radius: 14px; color: #fff; background: #252d45; font-size: 16px; }
    input[type="color"] { height: 52px; padding: 4px; border: 0; border-radius: 14px; background: #252d45; }
    .row { display: grid; grid-template-columns: 1fr auto; gap: 12px; align-items: center; }
    .value { min-width: 48px; padding: 8px 10px; border-radius: 12px; text-align: center; background: rgba(255,255,255,.08); }
    button { width: 100%; margin-top: 22px; padding: 14px 18px; border: 0; border-radius: 16px; background: linear-gradient(135deg, #55d6ff, #8b5cf6); color: #fff; font-size: 17px; font-weight: 700; cursor: pointer; }
    .status { margin-top: 16px; color: #8ff0b2; text-align: center; min-height: 22px; }
  </style>
</head>
<body>
  <main>
    <h1>WS2812 灯光控制</h1>
    <p>连接 ESP32-C3 热点后，在这里选择颜色和动态效果。</p>

    <label for="mode">灯光效果</label>
    <select id="mode">
      <option value="rainbow">彩虹流动</option>
      <option value="solid">固定颜色</option>
      <option value="breath">呼吸灯</option>
      <option value="chase">追逐灯</option>
      <option value="off">关闭</option>
    </select>

    <label for="color">颜色</label>
    <input id="color" type="color" value="#ff5000">

    <label for="brightness">亮度</label>
    <div class="row">
      <input id="brightness" type="range" min="0" max="255" value="80">
      <span class="value" id="brightnessValue">80</span>
    </div>

    <label for="speed">速度（数值越小越快）</label>
    <div class="row">
      <input id="speed" type="range" min="10" max="200" value="35">
      <span class="value" id="speedValue">35</span>
    </div>

    <button id="apply">应用到灯带</button>
    <div class="status" id="status"></div>
  </main>

  <script>
    const $ = (id) => document.getElementById(id);
    const fields = ["mode", "color", "brightness", "speed"];

    function refreshLabels() {
      $("brightnessValue").textContent = $("brightness").value;
      $("speedValue").textContent = $("speed").value;
    }

    async function applySettings() {
      refreshLabels();
      const params = new URLSearchParams();
      fields.forEach((field) => params.set(field, $(field).value));
      $("status").textContent = "发送中...";
      const response = await fetch(`/api/set?${params.toString()}`);
      $("status").textContent = response.ok ? "已更新" : "更新失败";
    }

    async function loadState() {
      const response = await fetch("/api/state");
      if (!response.ok) return;
      const state = await response.json();
      fields.forEach((field) => { if (state[field] !== undefined) $(field).value = state[field]; });
      refreshLabels();
    }

    $("brightness").addEventListener("input", refreshLabels);
    $("speed").addEventListener("input", refreshLabels);
    $("apply").addEventListener("click", applySettings);
    loadState();
  </script>
</body>
</html>
)rawliteral";

String colorToHex(const CRGB& color) {
  char buffer[8];
  snprintf(buffer, sizeof(buffer), "#%02X%02X%02X", color.r, color.g, color.b);
  return String(buffer);
}

CRGB hexToColor(String hex) {
  hex.trim();
  if (hex.startsWith("#")) {
    hex.remove(0, 1);
  }
  if (hex.length() != 6) {
    return currentColor;
  }

  long value = strtol(hex.c_str(), nullptr, 16);
  return CRGB((value >> 16) & 0xFF, (value >> 8) & 0xFF, value & 0xFF);
}

void showSolid(const CRGB& color) {
  fill_solid(leds, LED_COUNT, color);
  FastLED.show();
}

void updateLights() {
  if (millis() - lastFrameMs < currentSpeedMs) {
    return;
  }
  lastFrameMs = millis();

  FastLED.setBrightness(currentBrightness);

  if (currentMode == "off") {
    showSolid(CRGB::Black);
  } else if (currentMode == "solid") {
    showSolid(currentColor);
  } else if (currentMode == "breath") {
    CRGB color = currentColor;
    color.nscale8_video(beatsin8(18, 20, 255));
    showSolid(color);
  } else if (currentMode == "chase") {
    fadeToBlackBy(leds, LED_COUNT, 90);
    leds[animationStep % LED_COUNT] = currentColor;
    FastLED.show();
    animationStep++;
  } else {
    fill_rainbow(leds, LED_COUNT, animationStep, 12);
    FastLED.show();
    animationStep++;
  }
}

void handleRoot() {
  server.send_P(200, "text/html; charset=utf-8", INDEX_HTML);
}

void handleState() {
  String json = "{";
  json += "\"mode\":\"" + currentMode + "\",";
  json += "\"color\":\"" + colorToHex(currentColor) + "\",";
  json += "\"brightness\":" + String(currentBrightness) + ",";
  json += "\"speed\":" + String(currentSpeedMs);
  json += "}";
  server.send(200, "application/json", json);
}

void handleSet() {
  if (server.hasArg("mode")) {
    currentMode = server.arg("mode");
  }
  if (server.hasArg("color")) {
    currentColor = hexToColor(server.arg("color"));
  }
  if (server.hasArg("brightness")) {
    currentBrightness = constrain(server.arg("brightness").toInt(), 0, 255);
  }
  if (server.hasArg("speed")) {
    currentSpeedMs = constrain(server.arg("speed").toInt(), 10, 200);
  }

  lastFrameMs = 0;
  handleState();
}

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812B, WS2812_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(currentBrightness);
  FastLED.clear(true);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID, AP_PASSWORD);

  server.on("/", handleRoot);
  server.on("/api/state", handleState);
  server.on("/api/set", handleSet);
  server.onNotFound([]() {
    server.send(404, "text/plain; charset=utf-8", "页面不存在");
  });
  server.begin();

  Serial.println();
  Serial.println("ESP32-C3 WS2812 Web 控制器已启动");
  Serial.print("WiFi 热点：");
  Serial.println(AP_SSID);
  Serial.print("访问地址：http://");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
  updateLights();
}
