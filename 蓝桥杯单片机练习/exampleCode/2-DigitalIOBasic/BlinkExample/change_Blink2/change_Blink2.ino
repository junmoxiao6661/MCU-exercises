int ledPin = 13;
int delayTime = 1000;
void setup()
{
pinMode(ledPin,OUTPUT);
}
void loop()
{
digitalWrite(ledPin,HIGH); //点亮小灯
delay(delayTime); //延时
digitalWrite(ledPin,LOW); //熄灭小灯
delay(delayTime);
delayTime=delayTime-10; // 每次将延时时间减少0.1s
if(delayTime<10)
delayTime=1000; // 当延时时间小于0.1s 时，重新校准延时为1s
}
