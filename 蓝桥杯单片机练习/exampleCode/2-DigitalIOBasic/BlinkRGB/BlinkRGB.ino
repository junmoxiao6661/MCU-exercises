
#include <FastLED.h>
// How many leds in your strip?
#define NUM_LEDS 2
// Data controll pin
#define DATA_PIN 7
// Define the array of leds
uint8_t Bright=255;
CRGB leds[NUM_LEDS];
CRGB temp;
void setup() 
{ 
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
    leds[0] = CRGB::Red;leds[1] = CRGB::Green;leds[2] = CRGB::Blue;leds[3] = CRGB::Yellow;
    leds[4] = CRGB::Red;leds[5] = CRGB::Green;leds[6] = CRGB::Blue;leds[7] = CRGB::Yellow;
}
void loop() { 
  temp=leds[0];
  for(int i=0;i<NUM_LEDS-1;i++)
      leds[i]=leds[i+1];
  leds[NUM_LEDS-1]=temp;
  FastLED.setBrightness(Bright);
  FastLED.show();
  delay(200);
}
