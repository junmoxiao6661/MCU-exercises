int NorthSouthR=13,NorthSouthG=12,NorthSouthY=11;
int EastWestR=10,EastWestG=9, EastWestY=8;

void setup() {
  pinMode(NorthSouthR, OUTPUT);
  pinMode(NorthSouthG, OUTPUT);
  pinMode(NorthSouthY, OUTPUT);
  pinMode(EastWestR, OUTPUT);
  pinMode(EastWestG, OUTPUT);
  pinMode(EastWestY, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(NorthSouthR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(NorthSouthG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(NorthSouthY, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestR, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestG, HIGH);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestY, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(NorthSouthR, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(NorthSouthG, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(NorthSouthY, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestR, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestG, LOW);   // turn the LED on (HIGH is the voltage level)
  digitalWrite(EastWestY, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
}
