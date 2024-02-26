#include <STC15F2K60S2.H>

void Led_Display(unsigned char i,ena)
{
	static int j=0x00;
	static int k=0xff;
	if(ena)
		j|=(0x01<<i);
	else
		j&=~(0x01<<i);
	if(j!=k)
	{
		P0=~j;
		P2=P2&0x1F|0x80;
		P2=P2&0x1F;
		k=j;
	}
}
