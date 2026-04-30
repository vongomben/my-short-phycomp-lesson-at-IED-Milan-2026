void setup() {
  Serial.begin(9600);
}

void loop() {
  int a0Value = analogRead(A0);
  Serial.println(a0Value);
  delay(100);
}
