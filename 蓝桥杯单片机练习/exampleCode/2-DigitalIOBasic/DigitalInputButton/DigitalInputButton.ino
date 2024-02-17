int Switch = 5;       //定义输入引脚编号  
int Led1=8 ;      //定义输出引脚编号
int Led2=9 ;           
void setup() 
{
  Serial.begin(9600);
  pinMode(Switch, INPUT_PULLUP);
    pinMode(Led1, OUTPUT);pinMode(Led2, OUTPUT);
}
void loop() 
{
  int sensorVal = digitalRead(Switch);
  Serial.println(sensorVal);
  if (sensorVal == HIGH) 
  {
       digitalWrite(Led1, LOW);
       digitalWrite(Led2, LOW);
   } 
  else
  {
        digitalWrite(Led1, HIGH);digitalWrite(Led2, HIGH);
    }
   delay(500);
}
