// 05_display：先逐段點亮檢查接線，再從 0 數到 9
const int SEG[7] = {2, 3, 4, 5, 6, 7, 8};  // ① a b c d e f g 接的腳位
const byte DIGITS[10] = {  // ② 數字 0–9 各要亮哪幾段
  0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};

void show(byte p) {  // ③ 依照 p 點亮 a 到 g
  for (int i = 0; i < 7; i++)
    digitalWrite(SEG[i], (p >> i) & 1);
}

void setup() {
  for (int i = 0; i < 7; i++) pinMode(SEG[i], OUTPUT);
  // ④ 檢查接線：a 到 g 依序各亮 0.5 秒
  for (int i = 0; i < 7; i++) { show(1 << i); delay(500); }
}

void loop() {
  for (int n = 0; n <= 9; n++) { show(DIGITS[n]); delay(1000); }
}