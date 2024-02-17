#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;
sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void Delay(u16 x)		//@12.000MHz
{
	unsigned char i, j;
	while(x--)
	{
		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

u8 code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
void InitHC138(u8 n)
{
	switch(n)
	{
		case 0: HC138_C=0;HC138_B=0;HC138_A=0;break;
		case 1: HC138_C=0;HC138_B=0;HC138_A=1;break;
		case 2: HC138_C=0;HC138_B=1;HC138_A=0;break;
		case 3: HC138_C=0;HC138_B=1;HC138_A=1;break;
		case 4: P2=(P2&0x1f)|0x80;break;
		//P2=(P2&0x1f)|0x80;
		case 5: P2=(P2&0x1f)|0xa0;break;
		case 6: P2=(P2&0x1f)|0xc0;break;
		case 7: P2=(P2&0x1f)|0xe0;break;
	}
}
void DisplaySMG_Bit(u8 value,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=value;
	Delay(2);
	P0=0xFF;
	
}
u8 month=1;
void Display_Dynamic()
{
	DisplaySMG_Bit(SMG[2],0);
	DisplaySMG_Bit(SMG[0],1);
	DisplaySMG_Bit(SMG[2],2);
	DisplaySMG_Bit(SMG[3],3);
	DisplaySMG_Bit(SMG[10],4);
	DisplaySMG_Bit(SMG[10],5);
	DisplaySMG_Bit(SMG[month/10],6);
	DisplaySMG_Bit(SMG[month%10],7);
}
void DelaySMG(u16 t)
{
	while(t--)
	{
		Display_Dynamic();
	}
}
void main()
{
	
	while(1)
	{
		
		Display_Dynamic();
		month++;
		if(month>12)
			month=1;
		DelaySMG(100);
	}
}
