/*
  程序说明: DS1302驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include <reg52.h>
#include <intrins.h>

sbit SCK=P1^7;		
sbit SDA=P2^3;		
sbit RST = P1^3;   // DS1302复位												

void Write_Ds1302(unsigned  char temp) 
{
	unsigned char i;
	for (i=0;i<8;i++)     	
	{ 
		SCK=0;
		SDA=temp&0x01;
		temp>>=1; 
		SCK=1;
	}
}   

void Write_Ds1302_Byte( unsigned char address,unsigned char dat )     
{
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1; 	_nop_();  
 	Write_Ds1302(address);	
 	Write_Ds1302(dat);		
 	RST=0; 
}

unsigned char Read_Ds1302_Byte ( unsigned char address )
{
 	unsigned char i,temp=0x00;
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
 	RST=1;	_nop_();
 	Write_Ds1302(address);
 	for (i=0;i<8;i++) 	
 	{		
		SCK=0;
		temp>>=1;	
 		if(SDA)
 		temp|=0x80;	
 		SCK=1;
	} 
 	RST=0;	_nop_();
 	SCK=0;	_nop_();
	SCK=1;	_nop_();
	SDA=0;	_nop_();
	SDA=1;	_nop_();
	return (temp);			
}

void Set_Rtc(const unsigned char * ucRtc)
{
	Write_Ds1302_Byte(0x8E,0x00);// WP=0鍐欏叆 ,鍐欏叆鏁版嵁涓�0x00
	Write_Ds1302_Byte(0x80,ucRtc[2]);
	Write_Ds1302_Byte(0x82,ucRtc[1]);
	Write_Ds1302_Byte(0x84,ucRtc[0]);
	
	Write_Ds1302_Byte(0x8E,0x80);
	// WP=1淇濇姢 ,鍐欏叆鏁版嵁涓�0x80
}

void Read_Rtc(unsigned char *ucRtc)
{
	ucRtc[0]=Read_Ds1302_Byte (0x85);
	ucRtc[1]=Read_Ds1302_Byte (0x83);
	ucRtc[2]=Read_Ds1302_Byte (0x81);
}
