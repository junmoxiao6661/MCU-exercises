#include <STC15F2K60S2.H>
#include <INTRINS.H>

sbit tx=P1^0;
sbit rx=P1^1;
void Delay12us()		//@12.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 33;
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

unsigned int Wave_Data(char changedis,unsigned int speed)
{
	unsigned int time=0; // 超声波函数本质时计算周期时间
	Wave_Init();
	TMOD&=0x0f;
	TH1=TL1=0;
	TR1=1;
	while((rx==1)&&(TF1==0));
	TR1=0;
	if(TF1==0)
	{
		time=(TH1<<8|TL1);
		return time*(float)(speed/20000.0)+changedis;
	}
	else
	{
		TF1=0;
		return 0;
	}

}