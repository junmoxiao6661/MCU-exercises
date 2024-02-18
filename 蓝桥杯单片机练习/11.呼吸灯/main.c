#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;
sbit L1= P0^0;
u8 Tcount, dutyfactor, direct;
void Delay1ms(u16 xms)		//@12.000MHz
{
	while(xms--)
	{
		unsigned char i, j;

		i = 2;
		j = 239;
		do
		{
			while (--j);
		} while (--i);
	}
}


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
void Timer0_Isr(void) interrupt 1
{
	TL0 = 0x38;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	Tcount++;
}

void Timer0_Init(void)		//200微秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x38;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}

void BreatheLED()
{
	if(Tcount>=100)
	{
		Tcount=0;
		if(direct==0)
		{
			dutyfactor++;
		}
		else if(direct==1)
		{
			dutyfactor--;
		}
	}
	
	if(dutyfactor<=0)
	{
		direct=0;
	}
	else if(dutyfactor>=100)
	{
		direct=1;
	}
	if(Tcount < dutyfactor) {		//周期计数值<占空比
		L1 = 0;					//当前PWM周期的当前计数周期内灯亮
	} 
	else {
		L1 = 1;					//当前PWM周期的当前计数周期内灯灭
	}

	
}
void main()
{
	InitHC138(4);
	L1=1;
	Timer0_Init();
	while(1)
	{
		BreatheLED();
	}
}
