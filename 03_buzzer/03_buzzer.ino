// 03_buzzer：無源蜂鳴器播放〈小蜜蜂〉
const int BUZZER = 9;
// ① 簡譜 1–7（Do 到 Si）的頻率
const int FREQ[] = {0, 262, 294, 330, 349, 392, 440, 494};
const int BEAT_MS = 300;  // 一拍幾毫秒
// ② 簡譜：一個字一拍，'-' 是前一個音延長一拍
const char SONG[] = "533-422-1234555-533-422-13553---"
                    "2222234-3333345-533-422-13551---";

void setup() {
  for (int i = 0; SONG[i] != '\0'; i++) {
    if (SONG[i] == '-') continue;
    int beats = 1;
    while (SONG[i + beats] == '-') beats++;  // ③ 這個音有幾拍
    tone(BUZZER, FREQ[SONG[i] - '0'], beats * BEAT_MS * 0.9);
    delay(beats * BEAT_MS);
  }
}
void loop() {}  // 只播一次，按 RESET 再聽
