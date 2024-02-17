#include <STC15F2K60S2.H>
#include <INTRINS.H>
typedef unsigned char u8;
typedef unsigned int u16;
sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

//74HC138译码器工作原理为按CBA顺序输入0或1表示7个位置，111是Y7，本次用的Y4是110
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

void LEDRunning()
{
	u8 i;
	HC138_C=1;
	HC138_B=0;
	HC138_A=0;
	for(i=1;i<=3;i++)
	{
		P0=0x00;
		Delay(1000);
		P0=0xFF;
		Delay(1000);
	}
	for(i=1;i<=8;i++)
	{
		P0=(0xFF<<i);//最低位是第一个灯
		Delay(1000);
	}
	for(i=1;i<=8;i++)
	{
		P0=~(0xFF<<i);//最低位是第一个灯
		Delay(1000);
	}
}
void main()
{
	while(1)
	{
		LEDRunning();
	}
}
