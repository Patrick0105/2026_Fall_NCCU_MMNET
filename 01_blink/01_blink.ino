// 01_blink：讓板子上的 L 燈閃爍
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // ① L 燈設為輸出
}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);  // ② 亮
  delay(500);  // ③ 等 0.5 秒
  digitalWrite(LED_BUILTIN, LOW);  // 滅
  delay(500);
}
