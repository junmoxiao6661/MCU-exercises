// import the library in the sketch
#include <L293.h>
const int speedPin = 2;		// that is the pin that we use to control the motor's speed
const int forwardPin = 3;	// this is the pin that we use to tell the motor to go forward
const int reversePin = 4;	// this is the pin that we use to tell the motor to go reverse
L293 motor( speedPin, forwardPin, reversePin );
void setup()
{
	Serial.begin( 9600 );	//enable serial communication
  Serial.print("Motor test");
}
void loop()
{
	if ( Serial.available() > 0 )	//check if there is an incoming command on the serial line
	{
		String command = Serial.readString();	//store the command in a variable
		// select the proper action
		if ( command == "f" ) motor.forward( 255 );
		else if ( command == "r" ) motor.back( 255 );
		else if ( command == "s" ) motor.stop();
	}
}
