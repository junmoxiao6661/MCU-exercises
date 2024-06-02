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

unsigned char Wave_Data()
{
	unsigned int time;
	CMOD&=0x00;
	CH=CL=0;
	Wave_Init();
	CR=1;
	while(rx==1&&CF==0);
	CR=0;
	if(CF==0)
	{
		time=CH<<8|CL;
		return time*0.017;
	}
	else
	{
		CF=0;
		return 0;
	}
	
}

unsigned char Wave_Data()
{
	unsigned int time=0;
	CMOD&=0x00;
	CL=CH=0;
	Wave_Init();
	CR=1;
	while(rx==1&&CF==0);
	CR=0;
	if(CF==0)
	{
		time=CH<<8|CL;
		return time*0.017;
	}
	else
	{
		CF=0;
		return 255;
	}
}