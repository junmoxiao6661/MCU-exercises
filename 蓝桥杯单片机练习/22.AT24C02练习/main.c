#include <STC15F2K60S2.H>
#include "iic.h"
#include <INTRINS.H>
typedef unsigned char u8;
typedef unsigned int u16;
u8 code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
void Delay(u16 xms)		//@12.000MHz
{
	unsigned char i, j;
	while(xms--)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

void InitHC138(u8 n)
{
	switch(n)
	{
		case 0 : P2=(P2&0x1F)|0x00; break;
		case 4 : P2=(P2&0x1F)|0x80; break;
		case 5 : P2=(P2&0x1F)|0xA0; break;
		case 6 : P2=(P2&0x1F)|0xC0; break;
		case 7 : P2=(P2&0x1F)|0xE0; break;
	}
}
void ShowSMG(u8 val,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=val;
	Delay(1);
	P0=0xFF;
}
u16 temp=325;
u8 high=0,low=0;
u16 res;
u8 temp1[2];

void main()
{
	high=temp>>8;
	low=temp&0xFF;
	Write_EEPROM(&high,0,1);
	Delay(100);
	Write_EEPROM(&low,1,1);
	Delay(100);
	Read_EEPROM(temp1,0,2);
	res= (temp1[0]<<8)|temp1[1];
	while(1)
	{
		ShowSMG(SMG[res/100],0);
		ShowSMG(SMG[res/10%10],1);
		ShowSMG(SMG[res%10],2);
		
	}
}