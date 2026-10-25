// 02_led：紅黃綠燈輪流亮
const int GREEN = A0, YELLOW = A1, RED = A2;  // ① 幫腳位取名字

void setup() {
  pinMode(GREEN, OUTPUT);  // ② 三個腳都設為輸出
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
}

void loop() {
  digitalWrite(GREEN, HIGH); delay(1000);  // ③ 綠燈 1 秒
  digitalWrite(GREEN, LOW);
  digitalWrite(YELLOW, HIGH); delay(500);  // 黃燈 0.5 秒
  digitalWrite(YELLOW, LOW);
  digitalWrite(RED, HIGH); delay(2000);  // 紅燈 2 秒
  digitalWrite(RED, LOW);
}
