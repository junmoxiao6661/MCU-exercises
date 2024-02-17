#include <STC15F2K60S2.H>
#include <INTRINS.H>
typedef unsigned char u8;
typedef unsigned int u16;
sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void Delay(u16 t)
{
	while(t--);
	while(t--);
}
void LEDRunning()
{
	u8 i;
	HC138_C=1;
	HC138_B=0;
	HC138_A=0;
	/*for(i=1;i<=8;i++)
	{
		P0=0xFF<<i;
		Delay(60000);
		Delay(60000);
	}
	for(i=1;i<=8;i++)
	{
		P0=(0x01>>i)+1;
		Delay(60000);
		Delay(60000);
	}*/
	P0=0x80;
}
void main()
{
	while(1)
	{
		LEDRunning();
	}
}
