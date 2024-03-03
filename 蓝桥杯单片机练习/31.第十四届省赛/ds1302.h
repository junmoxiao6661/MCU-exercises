#ifndef __DS1302_H
#define __DS1302_H

void Write_Ds1302(unsigned char temp);
void Write_Ds1302_Byte( unsigned char address,unsigned char dat );
unsigned char Read_Ds1302_Byte( unsigned char address );
void Read_Rtc(unsigned char *rtc);
void Set_Rtc(unsigned char *rtc);
#endif
