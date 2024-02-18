#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;
sbit L1 =P0^0;
sbit L8 =P0^7;
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
void InitTimer0()
{
	TMOD &= 0x0F;
	TMOD |= 0x01;
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	ET0 =1; //T0定时器中断打开
	EA =1 ; //总中断打开
	TR0 = 1;//T0定时器开始计时
	
}
u8 count =0;
u8 count1 =0;
void ServiceTimer0() interrupt 1
{
	TH0 = (65535-50000) / 256;
	TL0 = (65535-50000) % 256;
	count++;
	count1++;
	if(count==10)
	{
		L1=~L1;//每0.5s取反一次，而不是直接复制0 1
		//LED连续变化取反
		count=0;
	}
	if(count1==100)
	{
		L8=~L8;
		count1=0;
	}
		
}
void main()
{
	InitHC138(4);
	InitTimer0();
	while(1)
	{
		
	}
}