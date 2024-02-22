#include <STC15F2K60S2.H>
#include "iic.h"

typedef unsigned char u8;
typedef unsigned int u16;
u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf};
sbit S4= P3^3;
u8 flag=0;
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

void InitHC138(u8 n)
{
	switch(n)
	{
		case 0: P2=(P2&0x1F)|0x00; break;
		case 4: P2=(P2&0x1F)|0x80; break;
		case 5: P2=(P2&0x1F)|0xA0; break;
		case 6: P2=(P2&0x1F)|0xC0; break;
		case 7: P2=(P2&0x1F)|0xE0; break;
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
u8 temp;
void Display()
{
	switch(flag)
	{
		case 0 :
			temp=Ad_Read(0x01);
			ShowSMG(SMG[10],0);
			ShowSMG(SMG[1],1);
			ShowSMG(SMG[10],2);
			ShowSMG(SMG[temp/100],5);
			ShowSMG(SMG[temp/10%10],6);
			ShowSMG(SMG[temp%10],7);
			break;
		case  1 :
			temp=Ad_Read(0x03);
			ShowSMG(SMG[10],0);
			ShowSMG(SMG[3],1);
			ShowSMG(SMG[10],2);
			ShowSMG(SMG[temp/100],5);
			ShowSMG(SMG[temp/10%10],6);
			ShowSMG(SMG[temp%10],7);
			break;

	}
}
void DelaySMG(u16 t)
{
	while(t--);
}
void ScanKey()
{
	if(S4==0)
	{
		DelaySMG(100);
		if(S4==0)
		{
			if(flag==0)
			{
				while(S4==0)
				{
					Display();
				}
				flag=1;
			}
			else if(flag==1)
			{
				while(S4==0)
				{
					Display();
				}
				flag=0;
			}
		}
	}
}
void main()
{
	InitHC138(4);
	P0=0xFF;
	while(1)
	{
		ScanKey();
		Display();
	}
}
