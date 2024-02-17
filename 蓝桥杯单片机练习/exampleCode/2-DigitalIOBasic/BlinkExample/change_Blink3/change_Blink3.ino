int ledPin = 13;
int delayTime = 1000;
int up=0;
int down=1;
void setup()
{
pinMode(ledPin,OUTPUT);
}
void loop()
{
digitalWrite(ledPin,HIGH);
delay(delayTime);
digitalWrite(ledPin,LOW);
delay(delayTime);
if(up==1)
{
delayTime=delayTime+100;
if(delayTime>1000) {
                    up=0;down=1;}
}
if(down==1)
{
delayTime=delayTime-100; // 大于或等于0.1s 时将延时时间缩短
if(delayTime<100) {
                    up=1;down=0;}
}
}
