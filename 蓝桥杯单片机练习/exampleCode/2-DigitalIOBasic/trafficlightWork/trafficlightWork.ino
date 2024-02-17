int NorthSouthR=13,NorthSouthG=11,NorthSouthY=12;
int EastWestR=10,EastWestG=8, EastWestY=9;

void setup() {
  pinMode(NorthSouthR, OUTPUT);
  pinMode(NorthSouthG, OUTPUT);
  pinMode(NorthSouthY, OUTPUT);
  pinMode(EastWestR, OUTPUT);
  pinMode(EastWestG, OUTPUT);
  pinMode(EastWestY, OUTPUT);
}
void NSRed(int State)
{
  if(State==1)
    digitalWrite(NorthSouthR, HIGH);
  else 
    digitalWrite(NorthSouthR, LOW);
}
void NSGreen(int State)
{
  if(State==1)
    digitalWrite(NorthSouthG, HIGH);
  else 
    digitalWrite(NorthSouthG, LOW);
}
void NSYellow(int State)
{
  if(State==1)
    digitalWrite(NorthSouthY, HIGH);
  else 
    digitalWrite(NorthSouthY, LOW);
}
void EWRed(int State)
{
  if(State==1)
    digitalWrite(EastWestR, HIGH);
  else 
    digitalWrite(EastWestR, LOW);
}
void EWGreen(int State)
{
  if(State==1)
    digitalWrite(EastWestG, HIGH);
  else 
    digitalWrite(EastWestG, LOW);
}
void EWYellow(int State)
{
  if(State==1)
    digitalWrite(EastWestY, HIGH);
  else 
    digitalWrite(EastWestY, LOW);
}
void HoldTime(int seconds)
{
  int i;
 for(i=0;i<seconds;i++)
 {
  delay(1000);
 }
}
int LightState=0;
void loop() 
{
  switch(LightState)
  {
    case 0:
      NSRed(0);NSGreen(1);NSYellow(0);
      EWRed(1);EWGreen(0);EWYellow(0);
      HoldTime(3);LightState++;
      break;
    case 1:
      NSRed(0);NSGreen(0);NSYellow(1);
      EWRed(1);EWGreen(0);EWYellow(0);
      HoldTime(1);LightState++;
      break;
    case 2:
      NSRed(1);NSGreen(0);NSYellow(0);
      EWRed(0);EWGreen(1);EWYellow(0);
      HoldTime(3);LightState++;
      break;
    case 3:
      NSRed(1);NSGreen(0);NSYellow(0);
      EWRed(0);EWGreen(0);EWYellow(1);
      HoldTime(1);LightState=0;
      break;
    default:LightState=0;break;
  }                
}
