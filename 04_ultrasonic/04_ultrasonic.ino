// 04_ultrasonic：超音波測距，結果顯示在序列埠監控視窗
const int TRIG = 13, ECHO = 12;
void setup() {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);  // ① 開啟序列埠，鮑率 9600
}
long readDistanceCm() {
  digitalWrite(TRIG, HIGH);  // ② 送出 10 微秒的聲波
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  long t = pulseIn(ECHO, HIGH, 30000);  // ③ 等回音，最多 30 毫秒
  if (t == 0) return 999;  // 沒收到回音：當作很遠
  return t / 58;  // ④ 微秒換成公分
}
void loop() {
  Serial.print(readDistanceCm()); Serial.println(" cm");
  delay(200);
}