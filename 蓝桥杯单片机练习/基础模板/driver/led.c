#include "led.h"
#include <STC15F2K60S2.H>
static unsigned char a=0x00;
static unsigned char b=0xff;
void LED_dis(unsigned char i,ena)
{
	static unsigned char j=0x00;
	static unsigned char k=0xff;
	if(ena)
	j |=(0x01<<i);
	else
	j &=~(0x01<<i);
	if(j!=k)
	{
		P0=~j;
		P2=P2&0x1f|0x80;
		P2&=0x1f;
		k=j;
	}
}

void beep_dis(unsigned char ena)
{

	if(ena)
	a |=0x40;
	else
	a &=~0x40;
	if(a!=b)
	{
		P0=a;
		P2=P2&0x1f|0xa0;
		P2&=0x1f;
		b=a;
	}
}

void realy_dis(unsigned char ena)
{

	if(ena)
	a |=0x10;
	else
	a &=~0x10;
	if(a!=b)
		{
		P0=a;
		P2=P2&0x1f|0xa0;
		P2&=0x1f;
		b=a;
	}
}
