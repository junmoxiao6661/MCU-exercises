#include <STC15F2K60S2.H>
#include <INTRINS.H>

sbit tx=P1^0;
sbit rx=P1^1;

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 38;
	while (--i);
}

void Wave_Init()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		tx=1;
		Delay12us();
		tx=0;
		Delay12us();
	}
}
