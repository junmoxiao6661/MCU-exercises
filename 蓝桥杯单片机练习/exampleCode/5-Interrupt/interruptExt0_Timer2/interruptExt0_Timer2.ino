#include <MsTimer2.h>
int pin = 2; //define interrupt pin to 2
volatile bool state = LOW; // To make sure variables shared between an ISR
void flashISR()
{
  static boolean output = HIGH; 
  digitalWrite(13, output);
  output = !output;
}
void blinkISR() { 
    if(state)
      MsTimer2::set(500, flashISR); // 500ms period
    else
      MsTimer2::set(100, flashISR); // 500ms period  
    MsTimer2::start(); 
    state = !state; //toggle the state when the interrupt occurs
}
void setup() {
   pinMode(13, OUTPUT); //set pin 13 as output
   attachInterrupt(digitalPinToInterrupt(pin), blinkISR, RISING);
} 
void loop() {   } 
  
