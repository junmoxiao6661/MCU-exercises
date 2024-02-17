#include <MsTimer2.h>
const int led_pin = LED_BUILTIN;
void flash()
{
  static boolean output = HIGH; 
  digitalWrite(led_pin, output);
  output = !output;
}
void setup()
{
  pinMode(led_pin, OUTPUT);
  MsTimer2::set(100, flash); // 500ms period
  MsTimer2::start();
}

void loop()
{
  //nothing!!!!
}
