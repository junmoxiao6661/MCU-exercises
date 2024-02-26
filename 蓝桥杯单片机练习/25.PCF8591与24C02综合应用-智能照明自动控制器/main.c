#include <STC15F2K60S2.H>
#include "iic.h"
typedef unsigned char u8;
typedef unsigned int u16;
sbit S4= P3^3;
sbit S5= P3^2;

u8 code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
u8 RD1=0;
u8 dat=0;
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

void KeyDelay(u16 t)
{
	while(t--);
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
unsigned char level()
{
	if(RD1>200&&RD1<=255)
	{
		InitHC138(4);
		P0=0xFE;
		InitHC138(0);
		return 1;
	}
	else if(RD1>150&&RD1<=200)
	{
		InitHC138(4);
		P0=0xFC;
		InitHC138(0);
		return 2;
	}
	else if(RD1>100&&RD1<=150)
	{
		InitHC138(4);
		P0=0xF0;
		InitHC138(0);
		return 3;
	}
	else if(RD1>50&RD1<=100)
	{
		InitHC138(4);
		P0=0xC0;
		InitHC138(0);
		return 4;
	}
	else if(RD1>0&&RD1<=50)
	{
		InitHC138(4);
		P0=0x00;
		InitHC138(0);
		return 5;
	}
	return 0;
}
void Display()
{
	ShowSMG(SMG[10],0);
	ShowSMG(SMG[level()],1);
	ShowSMG(SMG[10],2);
	
	ShowSMG(SMG[RD1/100],5);
	ShowSMG(SMG[RD1/10%10],6);
	ShowSMG(SMG[RD1%10],7);
}
void ScanKey()
{
	if(S4==0)
	{
		KeyDelay(100);
		if(S4==0)
		{
			EEPROM_Write(&RD1,0,1);
			while(S4==0)
			{
				Display();
			}	
		}
	}
	if(S5==0)
	{
		KeyDelay(100);
		if(S5==0)
		{
			u8 temp;
			EEPROM_Read(&temp,0,1);
			while(S5==0)
			{
				RD1=temp;
				Display();
			}
		}
	}
}
void main()
{
	InitHC138(4);
	P0=0xFF;
	InitHC138(0);
	while(1)
	{
		RD1=AD_Read(0x01);
		Display();
		ScanKey();
	}
}