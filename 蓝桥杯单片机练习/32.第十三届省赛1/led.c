#include <STC15F2K60S2.H>

void Led_Display(unsigned char i,ena)
{
	static unsigned char j=0x00;
	static unsigned char k=0xff;
	if(ena)
		j|=(0x01<<i);
	else
		j&=~(0x01<<i);
	if(j!=k)
	{
		P0=~j;
		P2=P2&0x1f|0x80;
		P2=P2&0x1f;
		k=j;
	}
}

void Beep(unsigned char flag)
{
	static unsigned char j=0x00;
	static unsigned char k=0xff;
	if(flag)
		j|=0x40;
	else
		j&=~0x40;
	if(j!=k)
	{
		P0=j;
		P2=P2&0x1f|0xa0;
		P2=P2&0x1f;
		k=j;
	}
}

void Relay(unsigned char flag)
{
	static unsigned char j=0x00;
	static unsigned char k=0xff;
	if(flag)
		j|=0x10;
	else
		j&=~0x10;
	if(j!=k)
	{
		P0=j;
		P2=P2&0x1f|0xa0;
		P2=P2&0x1f;
		k=j;
	}
}