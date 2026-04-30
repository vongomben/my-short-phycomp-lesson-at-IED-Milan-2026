/*
  D1 mini MQTT example
  - Publishes A0 value on: deviceN/light
  - Subscribes on:        deviceN/led
    Commands:
      on   -> LED full on
      off  -> LED off
      up   -> 0..255 in 10 ms steps
      down -> 255..0 in 10 ms steps
      wave -> 0..255..0 in 10 ms steps
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "Hotspot";
const char* WIFI_PASSWORD = "12345678";
const char* MQTT_HOST = "192.168.1.100";
const uint16_t MQTT_PORT = 1883;
const char* MQTT_USER = "";
const char* MQTT_PASSWORD = "";

const int DEVICE_N = 1;  // Change per board: 1, 2, 3...
const int LED_PIN = D4;  // Onboard LED (active LOW on D1 mini)

WiFiClient espClient;
PubSubClient mqttClient(espClient);

String topicLight;
String topicLed;
String clientId;

enum LedMode {
  MODE_IDLE,
  MODE_UP,
  MODE_DOWN,
  MODE_WAVE_UP,
  MODE_WAVE_DOWN
};

LedMode ledMode = MODE_IDLE;
int ledValue = 0;  // 0..255 logical brightness (0 off, 255 full on)

unsigned long lastLedStepMs = 0;
unsigned long lastSensorPublishMs = 0;
const unsigned long LED_STEP_MS = 10;
const unsigned long SENSOR_PUBLISH_MS = 200;

void applyLed(int value) {
  ledValue = constrain(value, 0, 255);
  // Active LOW PWM: 255 -> fully on, 0 -> off
  analogWrite(LED_PIN, 255 - ledValue);
}

void handleCommand(const String& command) {
  if (command == "on") {
    ledMode = MODE_IDLE;
    applyLed(255);
  } else if (command == "off") {
    ledMode = MODE_IDLE;
    applyLed(0);
  } else if (command == "up") {
    ledMode = MODE_UP;
    applyLed(0);
  } else if (command == "down") {
    ledMode = MODE_DOWN;
    applyLed(255);
  } else if (command == "wave") {
    ledMode = MODE_WAVE_UP;
    applyLed(0);
  }
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += static_cast<char>(payload[i]);
  }
  message.trim();
  message.toLowerCase();

  if (String(topic) == topicLed) {
    handleCommand(message);
  }
}

void ensureWifiConnected() {
  if (WiFi.status() == WL_CONNECTED) {
    return;
  }

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void ensureMqttConnected() {
  while (!mqttClient.connected()) {
    if (mqttClient.connect(clientId.c_str(), MQTT_USER, MQTT_PASSWORD)) {
      mqttClient.subscribe(topicLed.c_str());
    } else {
      delay(1000);
    }
  }
}

void updateLedAnimation() {
  unsigned long now = millis();
  if (now - lastLedStepMs < LED_STEP_MS) {
    return;
  }
  lastLedStepMs = now;

  switch (ledMode) {
    case MODE_UP:
      if (ledValue < 255) {
        applyLed(ledValue + 1);
      } else {
        ledMode = MODE_IDLE;
      }
      break;
    case MODE_DOWN:
      if (ledValue > 0) {
        applyLed(ledValue - 1);
      } else {
        ledMode = MODE_IDLE;
      }
      break;
    case MODE_WAVE_UP:
      if (ledValue < 255) {
        applyLed(ledValue + 1);
      } else {
        ledMode = MODE_WAVE_DOWN;
      }
      break;
    case MODE_WAVE_DOWN:
      if (ledValue > 0) {
        applyLed(ledValue - 1);
      } else {
        ledMode = MODE_IDLE;
      }
      break;
    case MODE_IDLE:
    default:
      break;
  }
}

void publishA0Value() {
  unsigned long now = millis();
  if (now - lastSensorPublishMs < SENSOR_PUBLISH_MS) {
    return;
  }
  lastSensorPublishMs = now;

  int a0Value = analogRead(A0);
  char payload[8];
  snprintf(payload, sizeof(payload), "%d", a0Value);
  mqttClient.publish(topicLight.c_str(), payload, true);
}

void setup() {
  analogWriteRange(255);
  pinMode(LED_PIN, OUTPUT);
  applyLed(0);

  topicLight = "device" + String(DEVICE_N) + "/light";
  topicLed = "device" + String(DEVICE_N) + "/led";
  clientId = "d1mini-device" + String(DEVICE_N);

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);

  ensureWifiConnected();
}

void loop() {
  ensureWifiConnected();
  ensureMqttConnected();
  mqttClient.loop();

  updateLedAnimation();
  publishA0Value();
}
