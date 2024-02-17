#include <TM1650.h>
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h>
TM1650 module(A3, A2);  // SDA=A3, SCL=A2
TM16xxButtons buttons(&module);    // TM16xx button 
TM16xxDisplay display(&module, 4); // TM16xx object, 4 digits
void setup()
{
  module.clearDisplay();     // clear display
  module.setupDisplay(true, 3);  // set intensity 0-7, 7=highest
  module.setDisplayToString("HALO");
  delay(500);
  // Attach the button callback functions that are defined below
  buttons.attachClick(fnClick);
}
int start=0,starttime=0,internal=0;
void loop()
{
  uint32_t dwButtons=buttons.tick();
  if(start==1)
  {
    internal=(millis()-starttime)/100;
    display.println(internal);
  }
  else
  {
    display.println(internal);
  }
  delay(200); // to keep the loop fast don't use delay(100) 
}
void fnClick(byte nButton)
{
  if(nButton==13)
  {start=1;internal=0;starttime=millis();}
  else start=0;
} // click
