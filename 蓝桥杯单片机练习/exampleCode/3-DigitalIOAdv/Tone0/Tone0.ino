#include "pitches.h"
//基本音符:
int melody[] ={NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int SoundPin=6;
// 4代表四分音符，8代表8分音符，以此類推:
int noteDurations[] = { 4, 8, 8, 4, 4, 4, 4, 4};
void setup() 
{
  
}
void loop() {
  
  for (int thisNote = 0; thisNote < 8; thisNote++) 
  {
    //例如，四分音符播放時間為1000 / 4,八分音符播放時間為=1000/8,以此類推。
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(SoundPin, melody[thisNote], noteDuration);
    // 為了讓人耳能夠分辨清楚每個音符，設置播放每個音符間的最小間隔時間。音符的播放時間*1.3作為間隔時間是比較好的選擇:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(SoundPin);
  }
  
  tone(13,3000,500);
  tone(13,40,500);
  // no need to repeat the melody.
}
