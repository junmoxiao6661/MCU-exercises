#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void InitHC138(u8 n)
{
	switch(n)
	{
		case 0: HC138_C=0;HC138_B=0;HC138_A=0;break;
		case 1: HC138_C=0;HC138_B=0;HC138_A=1;break;
		case 2: HC138_C=0;HC138_B=1;HC138_A=0;break;
		case 3: HC138_C=0;HC138_B=1;HC138_A=1;break;
		case 4: HC138_C=1;HC138_B=0;HC138_A=0;break;
		//P2=(P2&0x1f)|0x80;
		case 5: HC138_C=1;HC138_B=0;HC138_A=1;break;
		case 6: HC138_C=1;HC138_B=1;HC138_A=0;break;
		case 7: HC138_C=1;HC138_B=1;HC138_A=1;break;
	}
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

void InitSystem()
{
	InitHC138(4);
	P0=0x00;
}
void OutPutP0(u8 channel,u8 dat)
{
	InitHC138(channel);
	P0=dat;
}
void Running()
{
	u8 i;
	InitHC138(4);
	for(i=1;i<=8;i++)
	{
		P0=0xFF<<i;
		Delay(1000);
	}
	InitHC138(5);
	P0=0x10;
	Delay(2000);
	P0=0x00;
	InitHC138(4);
	for(i=1;i<=8;i++)
	{
		P0=~(0xFF<<i);
		Delay(1000);
	}
	InitHC138(5);
	P0=0x40;
	Delay(2000);
	P0=0x00;
}
void main()
{
	InitSystem();
	while(1)
	{
		Running();
	}
}
