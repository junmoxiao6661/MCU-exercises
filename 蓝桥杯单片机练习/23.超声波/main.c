#include <STC15F2K60S2.H>

#include "ultrasonic.h"

typedef unsigned char u8;
typedef unsigned int u16;
sbit TX = P1^0;
sbit RX = P1^1;
u8 code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};

void Delay(u16 xms)		//@12.000MHz
{
	while(xms--)
	{
		unsigned char i, j;

		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}
void Delay1(u16 t)
{
	while(t--);
}
void InitHC138(u8 n)
{
	switch(n)
	{
		case 0 :P2=(P2&0x1F)|0x00; break;
		case 4 :P2=(P2&0x1F)|0x80; break;
		case 5 :P2=(P2&0x1F)|0xA0; break;
		case 6 :P2=(P2&0x1F)|0xC0; break;
		case 7 :P2=(P2&0x1F)|0xE0; break;
	}
}

void ShowSMG(u8 val,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=val;
	Delay(2);
	P0=0xFF;
}
u16 distance = 0;
void Display()
{
	ShowSMG(SMG[distance/100],0);
	ShowSMG(SMG[distance/10%10],1);
	ShowSMG(SMG[distance%10],2);
}
void DelayDisplay(u8 t)		//@12.000MHz
{
	while(t--)
	{
		Display();
	}
}

void main()
{
	while(1)
	{
		distance = Wave_Read();
		DelayDisplay(15);	
	}
}