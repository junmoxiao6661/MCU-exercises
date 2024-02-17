#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;
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
void ShowSMG_Bit(u8 dat,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=dat;
}
void Delay(u16 xms)		//@11.0592MHz
{
	unsigned char i, j;
	while(xms--)
	{
		_nop_();
		i = 2;
		j = 199;
		do
		{
		 	while (--j);
		} while (--i);
	}
}
void SMG_Static()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		for(j=0;j<=10;j++)
		{
			ShowSMG_Bit(SMG[j],i);
			Delay(1000);
		}
	}
	for(i=0;i<=10;i++)
	{
		InitHC138(6);
		P0=0xFF;
		InitHC138(7);
		P0=SMG[i];
		Delay(1000);
	}
}
void main()
{
	
	while(1)
	{
		SMG_Static();
	}
}
