const int LED_PIN = D4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // On D1 mini, onboard LED is active LOW.
  digitalWrite(LED_PIN, LOW);
  delay(1500);
  digitalWrite(LED_PIN, HIGH);
  delay(1500);
}
