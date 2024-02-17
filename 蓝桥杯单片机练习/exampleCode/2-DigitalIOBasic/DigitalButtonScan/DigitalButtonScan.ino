const int buttonPin1 = 2;    // the number of the pushbutton pin
const int buttonPin2 = 3;    // the number of the pushbutton pin
const int buttonPin3 = 4;    // the number of the pushbutton pin
const int buttonPin4 = 5;    // the number of the pushbutton pin
const int ledPin[4] = {13,12,11,10};      // the number of the LED pin
int ledState[4] = {LOW,LOW,LOW,LOW};         // the current state of the output pin
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
  for(int i=0;i<4;i++) 
  {
    pinMode(ledPin[i], OUTPUT);
    digitalWrite(ledPin[i], ledState[i]);
  }
}
int keyScan()
{
  for(int i=2;i<6;i++)
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
    ledState[keyValue-1] = !ledState[keyValue-1];
    digitalWrite(ledPin[keyValue-1],ledState[keyValue-1] );
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
      KeyValue=buttonPin-1; return KeyValue;
    }
  }
   return 0;
}
