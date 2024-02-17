int ledPin = 13;
int delayTime = 1000; // 定义延时变量delayTime 为1s
int delayTime2 = 2000; // 定义延时变量delayTime2 为2s
int count=0; // 定义计数器变量并初始化为0
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
count++;
if(count==10)
{
delay(delayTime2); // 当计数器数值为20 时，延时3s
}
}
