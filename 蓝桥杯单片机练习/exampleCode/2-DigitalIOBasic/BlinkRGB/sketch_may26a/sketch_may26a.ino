void setup() 
{ 
    pinMode(buttonPin1, INPUT);
    pinMode(buttonPin2, INPUT);
    pinMode(buttonPin3, INPUT);
    pinMode(buttonPin4, INPUT);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  
    leds[0] = CRGB::Red;leds[1] = CRGB::Green;
    leds[2] = CRGB::Blue;leds[3] = CRGB::White;
    FastLED.setBrightness(5);
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
void loop() 
{
  int keyValue=keyScan();
  if(keyValue>0) 
  {
    switch(keyValue)
    {case 1:
          setShowColor(CRGB::Red);
          break;
    case 2:
          setShowColor(CRGB::Blue);
          break;
    case 3:
          setShowColor(CRGB::White);
          break;
    case 4:
          setShowColor(CRGB::Green);
          break;           
      }
    FastLED.setBrightness(10);
    FastLED.show();  
  }
}
