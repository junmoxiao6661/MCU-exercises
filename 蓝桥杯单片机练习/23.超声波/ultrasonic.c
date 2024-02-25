#include <STC15F2K60S2.H>
#include <INTRINS.H>
sbit TX = P1^0;
sbit RX = P1^1;

void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
	while (--i);
}

void Wave_Send()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		TX=1;
		Delay12us();
		TX=0;
		Delay12us();
	}
}

unsigned int Wave_Read()
{
	unsigned int time;
	TMOD&=0x0F;
	TH1=TL1=0;
	Wave_Send();
	TR1=1;
	while((RX==1)&&(TF1==0));//=1未收到数据，TF=0没有溢出
	TR1=0;
	if(TF1==0)
	{
		time=TH1<<8|TL1;
		return (time*0.017);
	}
	else
	{
		TF1=0;
		return 0;
	}
	
}
