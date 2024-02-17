#include <Wire.h>
#include "DS1307.h"
#include "HD44780_LCD_PCF8574.h"
DS1307 clock;//define a object of DS1307 class
HD44780LCD myLCD( 2, 16, 0x27,&Wire); // instantiate an object
void setup()
{
	Serial.begin(9600);
	clock.begin();
	clock.fillByYMD(2022,4,14);
	clock.fillByHMS(19,10,35);
	clock.fillDayOfWeek(THU);
	clock.setTime();//write time to the RTC chip
  //myLCD.PCF8574_LCDInit(LCDCursorTypeOff);
  myLCD.PCF8574_LCDClearScreen();
  myLCD.PCF8574_LCDBackLightSet(true);

}
void loop()
{
	printTime();
  delay(500);
}
/*Function: Display time on the serial monitor*/
void printTime()
{
	char hour[3],minute[3],second[3],month[3],dayOfMonth[3],year[5];
	clock.getTime();
  sprintf(hour,"%02d",clock.hour);
  sprintf(minute,"%02d",clock.minute);
  sprintf(second,"%02d",clock.second);
  sprintf(month,"%02d",clock.month);
  sprintf(dayOfMonth,"%02d",clock.dayOfMonth);
  sprintf(year,"%d",clock.year+2000);
  myLCD.PCF8574_LCDGOTO(2,4);
  myLCD.PCF8574_LCDSendString(hour); 
  myLCD.PCF8574_LCDSendChar(':'); 
  myLCD.PCF8574_LCDSendString(minute); 
  myLCD.PCF8574_LCDSendChar(':'); 
  myLCD.PCF8574_LCDSendString(second);  
  myLCD.PCF8574_LCDGOTO(1,3);
  myLCD.PCF8574_LCDSendString(year); 
  myLCD.PCF8574_LCDSendChar('-'); 
  myLCD.PCF8574_LCDSendString(month);
  myLCD.PCF8574_LCDSendChar('-'); 
  myLCD.PCF8574_LCDSendString(dayOfMonth);    
	Serial.print(clock.hour, DEC);
	Serial.print(":");
	Serial.print(clock.minute, DEC);
	Serial.print(":");
	Serial.print(clock.second, DEC);
	Serial.print("	");
	Serial.print(clock.month, DEC);
	Serial.print("/");
	Serial.print(clock.dayOfMonth, DEC);
	Serial.print("/");
	Serial.print(clock.year+2000, DEC);
	Serial.print(" ");
	Serial.print(clock.dayOfMonth);
	Serial.print("*");
	switch (clock.dayOfWeek)// Friendly printout the weekday
	{
		case MON:
		  Serial.print("MON");
		  break;
		case TUE:
		  Serial.print("TUE");
		  break;
		case WED:
		  Serial.print("WED");
		  break;
		case THU:
		  Serial.print("THU");
		  break;
		case FRI:
		  Serial.print("FRI");
		  break;
		case SAT:
		  Serial.print("SAT");
		  break;
		case SUN:
		  Serial.print("SUN");
		  break;
	}
	Serial.println(" ");
}
