#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit L1=P0^0;
sbit L8=P0^7;

void Delay(u16 xms)		//@11.0592MHz
{
	while(xms--)
	{
		unsigned char i, j;

		_nop_();
		i = 2;
		j = 199;
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
		case 4 : P2=(P2&0x1F)|0x80; break;
		case 5 : P2=(P2&0x1F)|0xA0; break;
		case 6 : P2=(P2&0x1F)|0xC0; break;
		case 7 : P2=(P2&0x1F)|0xE0; break;
	}
}

void Working()
{
	InitHC138(4);
	L1=0;
	Delay(1000);
	L1=1;
	Delay(1000);
}
void Init_INT0()
{
	IT0 = 1;
	EX0 = 1; // 外部中断 0 中断允许位， EX0=1，允许外部中断 0 中断； EX0=0，禁止外部中断 0 中断
	EA = 1; //总中断
}
void ServiceINT0() interrupt 0
{
	L8 = 0;
	Delay(5000);
	L8 = 1;
}
void main()
{
	Init_INT0();
	while(1)
	{
		Working();
	}
}
