int ledPin = 13;
int delayTime = 1000; // 定义延时变量delayTime 为1s
int delayTime2 = 2000; // 定义延时变量delayTime2 为2s
void setup()
{
pinMode(ledPin,OUTPUT);
}
void loop()
{
  for(int count=0;count<10;count ++) //执行20 次延时3s
  {
    digitalWrite(ledPin,HIGH);
    delay(delayTime);
    digitalWrite(ledPin,LOW);
    delay(delayTime);
  }
  if(delayTime<100)
  {
    delayTime=1000; // 当延时小于0.1s 时校准延时时间为1s
  }
  else
  {
    delayTime=delayTime-100; // 大于或等于0.1s 时将延时时间缩短
  }
}
