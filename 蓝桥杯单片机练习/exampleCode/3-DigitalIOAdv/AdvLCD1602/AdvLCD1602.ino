// include the library code:
#include <LiquidCrystal.h>
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}
int i=0;
void loop() {
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  i++;
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
  lcd.setCursor(7, 1);
  lcd.print(i);
}
