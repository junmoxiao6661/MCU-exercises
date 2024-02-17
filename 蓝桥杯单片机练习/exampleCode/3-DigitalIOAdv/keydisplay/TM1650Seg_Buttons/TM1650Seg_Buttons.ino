#include <TM1650.h>
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h>
TM1650 module(A3, A2);  // SDA=A3, SCL=A2
TM16xxButtons buttons(&module);       // TM16xx button 
TM16xxDisplay display(&module, 4);    // TM16xx object, 4 digits
void setup()
{
  Serial.begin(115200);
  Serial.println(F("TM16xxButtons example"));
  Serial.println(F("TM1650:  DIO=5, CLK=4"));
  Serial.println();
  module.clearDisplay();              // clear display
  module.setupDisplay(true, 3);       // set intensity 0-7, 7=highest
  delay(50);

  // Show some text.
  // Note: the TM16xxDisplay provides the println() function which is used in the callback functions below.
  Serial.println(F("txt"));
  module.setDisplayToString("HALO");
  delay(400);
  module.setDisplayToString("Hi--");
  delay(400);
  // Attach the button callback functions that are defined below
  buttons.attachClick(fnClick);
  Serial.println(F("Setup done"));
}
void loop()
{
  uint32_t dwButtons=buttons.tick();
  delay(100); // to keep the loop fast don't use delay(100) 
}
// Button callback functions
// This function will be called when a button was pressed 1 time (without a second press).
void fnClick(byte nButton)
{
  Serial.print(F("Button "));
  Serial.print(nButton);
  Serial.println(F(" click."));
  display.print(" ");
  display.println(nButton);
} // click
