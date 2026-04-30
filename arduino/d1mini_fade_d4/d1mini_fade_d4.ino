const int LED_PIN = D4;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  analogWriteRange(255);
}

void loop() {
  // On D1 mini, onboard LED is active LOW.
  for (int value = 0; value <= 255; value++) {
    analogWrite(LED_PIN, 255 - value);
    delay(300);
  }

  for (int value = 255; value >= 0; value--) {
    analogWrite(LED_PIN, 255 - value);
    delay(300);
  }
}
