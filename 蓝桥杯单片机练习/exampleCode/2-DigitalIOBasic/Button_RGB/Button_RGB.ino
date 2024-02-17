
#include <FastLED.h>
// How many leds in your strip?
#define NUM_LEDS 4
// Data controll pin
#define DATA_PIN 7
// Define the array of leds
uint8_t Bright=10;
CRGB leds[NUM_LEDS];
CRGB temp;
const int buttonPin1 = 2;    // the number of the pushbutton pin
const int buttonPin2 = 3;    // the number of the pushbutton pin
const int buttonPin3 = 4;    // the number of the pushbutton pin
const int buttonPin4 = 5;    // the number of the pushbutton pin
const int ledPin[4] = {13,12,11,10};      // the number of the LED pin
int buttonState;             // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers
void setup() 
{ 
    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(buttonPin3, INPUT);
    pinMode(buttonPin4, INPUT);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    leds[0] = CRGB::Red;leds[1] = CRGB::Green;leds[2] = CRGB::Blue;leds[3] = CRGB::White;
    FastLED.setBrightness(2);
    FastLED.show();
}
int setShowColor(CRGB color)
{
  for(int i=0;i<NUM_LEDS;i++)
  {
    leds[i]=color;
  }
  return 0;
}
int keyScan()
{
  for(int i=0;i<4;i++)
  {
    int keyValue=buttonScan(i);
    if(keyValue>0){return keyValue;}
  }
  return 0;
}
void loop() 
{
  int keyValue=0;
  keyValue=keyScan();
  if(keyValue>0) 
  {
    switch(keyValue)
    {case 2:
          setShowColor(CRGB::Red);
          break;
    case 3:
          setShowColor(CRGB::Blue);
          break;
    case 4:
          setShowColor(CRGB::White);
          break;
    case 5:
          setShowColor(CRGB::Green);
          break;           
      }
    FastLED.setBrightness(3);
    FastLED.show();  
  }
}
int buttonScan(int buttonPin)
{
  int KeyValue;
  int reading = digitalRead(buttonPin);
  if (reading == LOW) 
  {
    lastDebounceTime = millis();
    while ((millis() - lastDebounceTime)<debounceDelay){;} 
    reading = digitalRead(buttonPin);
    if (reading == LOW)  
    { 
      do{reading = digitalRead(buttonPin);
      }while(reading==LOW);
      KeyValue=buttonPin+1; return KeyValue;
    }
  }
   return 0;
}
