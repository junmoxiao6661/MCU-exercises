// For more information see  https://github.com/maxint-rd/TM16xx
#include <TM1637.h>
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h>
//TM1637(byte dataPin, byte clockPin, byte numDigits, boolean activateDisplay, byte intensity)
TM1637 module(5, 4, 6);  // DIO=5, CLK=4
//TM16xxDisplay display(&module, 6);    // TM16xx object, 6 digits
TM16xxButtons buttons(&module);       // TM16xx button
void setup()
{
  Serial.begin(9600);
  Serial.println("Setup start");
  module.clearDisplay();
  module.setupDisplay(true, 7);
  buttons.attachClick(fnClick);
}
int keyPos=0;
char dispbuf[6]={'0','1' ,'2','3','4','5'};
void loop()
{ char key; uint16_t keybit;
  module.setDisplayToString(dispbuf);
  uint16_t  btButtons=buttons.tick();     // only need bits 0-3
  delay(10);
}
// This function will be called when a button was pressed 1 time (without a second press).
void fnClick(byte nButton)
{ char keyName[16]={'0','1','2','3','4','5','6','7','8','9','A','b','C','d','E','F'};
  if(nButton<16)
  { dispbuf[keyPos]=keyName[nButton];
    Serial.print(F("Button "));
    Serial.print(nButton);
    Serial.println(F(" click."));
    Serial.print("Name ");
    Serial.println(dispbuf[keyPos]);
    keyPos++;
    if(keyPos==6) keyPos=0;
  }
} // click
