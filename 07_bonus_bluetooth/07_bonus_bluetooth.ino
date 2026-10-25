
 // 07_bonus_bluetooth：倒車雷達＋藍牙，將距離傳送到手機（適合已完成 06_radar 的組別）

#include <SoftwareSerial.h>  // Bonus ①：引入軟體序列埠函式庫
SoftwareSerial BT(10, 11);  // Bonus ①：D10 接收、D11 傳送；若只傳資料到手機，D10 可不接
const char NAME[] = "group99";  // Bonus ②：修改為你們組的號碼

// ---------- 腳位 ----------
const int GREEN = A0, YELLOW = A1, RED = A2;  // 綠、黃、紅三色 LED
const int SEG[7] = {2, 3, 4, 5, 6, 7, 8};  // 七段顯示器的 a、b、c、d、e、f、g 段
const int BUZZER = 9;  // 無源蜂鳴器
const int ECHO = 12, TRIG = 13;  // 超音波感測器的回音接收與觸發腳位

// ---------- 可自行調整的參數 ----------
const bool COMMON_ANODE = false;  // 共陰極設為 false，共陽極設為 true
const int NEAR = 20;  // 距離小於此值（公分）時亮紅燈，蜂鳴器近似連續鳴叫
const int FAR = 50;  // 距離小於此值且不小於 NEAR 時亮黃燈，蜂鳴器間歇鳴叫

const byte DIGITS[10] = {
  0x3F, 0x06, 0x5B, 0x4F, 0x66,
  0x6D, 0x7D, 0x07, 0x7F, 0x6F
};  // 數字 0～9 對應的七段顯示器段碼
const byte DASH = 0x40;  // 只亮中間的 g 段，顯示橫線
const long NO_ECHO = 999;  // 未在逾時前收到有效回音時使用的特殊值
long readings[5] = {NO_ECHO, NO_ECHO, NO_ECHO, NO_ECHO, NO_ECHO};  // 儲存最近 5 次量測結果
int idx = 0;  // 下一筆量測要寫入的陣列位置
long distance = NO_ECHO;  // 目前採用的距離值
unsigned long lastMeasure = 0, lastBeep = 0;  // 記錄上次量測與蜂鳴器鳴叫的時間

void setup() {
  pinMode(GREEN, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(RED, OUTPUT);
  for (int i = 0; i < 7; i++) pinMode(SEG[i], OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  Serial.begin(9600);  // 設定電腦序列埠的傳輸速率
  BT.begin(9600);  // Bonus ③：設定藍牙序列埠速率；須與藍牙模組的設定一致
}

// 和模組 4 一樣：利用超音波回音時間計算距離
long readDistanceCm() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);  // 觸發前先讓 TRIG 維持低電位
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);  // 輸出 10 微秒的觸發脈衝
  digitalWrite(TRIG, LOW);

  long t = pulseIn(ECHO, HIGH, 30000);  // 等待回音高電位，最長等待 30 毫秒
  if (t == 0) return NO_ECHO;  // 逾時仍未收到有效脈衝
  return t / 58;  // 將回音時間約換算成公分
}

// 將最近 5 次量測結果排序，取中位數以降低少數異常值的影響
long median5() {
  long s[5];
  for (int i = 0; i < 5; i++) s[i] = readings[i];

  for (int i = 0; i < 5; i++)  // 由小到大排序
    for (int j = i + 1; j < 5; j++)
      if (s[j] < s[i]) {
        long t = s[i];
        s[i] = s[j];
        s[j] = t;  // 交換兩個數值
      }

  return s[2];  // 回傳排序後位於中間的數值
}

// 和模組 5 一樣：依照段碼控制七段顯示器
void show(byte p) {
  for (int i = 0; i < 7; i++)
    digitalWrite(SEG[i], ((p >> i) & 1) != COMMON_ANODE);
}

// 依距離控制蜂鳴器：越近，鳴叫間隔越短
void beep(unsigned long now) {
  if (distance < NEAR) {
    tone(BUZZER, 2000, 45);  // 紅燈區：每次量測都觸發 45 毫秒短音，聽起來接近連續鳴叫
  } else if (distance < FAR) {
    long interval = map(distance, NEAR, FAR, 120, 600);  // 距離越近，鳴叫間隔越短
    if (now - lastBeep >= interval) {
      lastBeep = now;
      tone(BUZZER, 1000, 45);  // 黃燈區：每次鳴叫持續 45 毫秒
    }
  }
}

// Bonus ④：將同一筆距離資料輸出到電腦序列埠，也傳送到手機
void sendDistance() {
  if (distance == NO_ECHO)
    Serial.println("沒收到回音");
  else {
    Serial.print(distance);
    Serial.println(" cm");
  }

  BT.print(NAME);
  BT.print(" ");
  if (distance == NO_ECHO)
    BT.println("--");  // 未收到有效回音時，手機顯示橫線
  else {
    BT.print(distance);
    BT.println(" cm");
  }
}

void loop() {
  unsigned long now = millis();

  if (now - lastMeasure >= 60) {  // ① 每隔約 0.06 秒量測一次距離
    lastMeasure = now;
    readings[idx] = readDistanceCm();
    idx = (idx + 1) % 5;  // 循環使用 5 個儲存位置

    distance = median5();  // 取最近 5 次量測結果的中位數

    show(distance < 100 ? DIGITS[distance / 10] : DASH);
    // ② 七段顯示器：顯示距離的十位數；距離達 100 公分或無有效回音時顯示橫線

    digitalWrite(GREEN, distance >= FAR);  // ③ 三色燈：依距離亮起綠燈、黃燈或紅燈
    digitalWrite(YELLOW, distance >= NEAR && distance < FAR);
    digitalWrite(RED, distance < NEAR);

    sendDistance();  // ④ 將量測結果輸出到電腦，並傳送到手機

    beep(now);  // ⑤ 依距離控制蜂鳴器的鳴叫頻率與間隔
  }
}
