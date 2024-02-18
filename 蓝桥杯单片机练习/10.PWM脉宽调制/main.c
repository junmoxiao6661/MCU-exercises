#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit L1=P0^0;
sbit S7=P3^0;

void Delay(u16 xms)		//@12.000MHz
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
u8 count=0;
u8 pwm_duty=0;
u8 stat = 0;

void Timer0_Init(void)		//100微秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0xF6;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值

	TF0 = 0;				//清除TF0标志
	//TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}

void Timer0_Isr(void) interrupt 1
{
	TL0 = 0xF6;				//设置定时初始值
	TH0 = 0xFF;				//设置定时初始值

	count++;
	if(count<=pwm_duty)
	{
		L1=0;
	}
	else if(count<=100)
	{
		L1=1;
	}
	else if(count==100)
	{
		L1=0;
		count=0;
	}
}

void ScanKeys()
{
	if(S7==0)
	{
		Delay(10);
		if(S7==0)
		{
			switch(stat)
			{
				case 0:
					L1=0;
					TR0=1;
					pwm_duty=25;
					stat=1;
				break;
				
				case 1:
					pwm_duty=50;
					stat=2;
				break;
				
				case 2:
					pwm_duty=75;
					stat=3;
				break;
				
				case 3:
					pwm_duty=100;
					stat=4;
				break;
				case 4:
					L1=1;
					TR0=0;
					stat=0;
				break;
			}
			while(S7==0);
		}
	}
}
void main()
{
	
	InitHC138(4);
	L1=1;
	Timer0_Init();
	while(1)
	{
		ScanKeys();
	}
}