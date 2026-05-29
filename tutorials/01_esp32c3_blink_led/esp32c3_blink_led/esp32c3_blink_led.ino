// 教程 1：ESP32-C3 使用 GPIO2 点亮普通 LED

const int LED_PIN = 2;
const unsigned long BLINK_INTERVAL_MS = 500;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  delay(BLINK_INTERVAL_MS);

  digitalWrite(LED_PIN, LOW);
  delay(BLINK_INTERVAL_MS);
}
