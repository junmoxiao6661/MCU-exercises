#include <STC15F2K60S2.H>
#include "seg.h"


typedef unsigned char u8;
typedef unsigned int u16;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u16 delay,fre;
u8 mode;
void Seg_Pro()
{
	unsigned char i=3;
	if(segslow) return;
	segslow=1;
	switch(mode)
	{
		case 0 :
			seg[3]=fre/10000;
			seg[4]=fre/1000%10;
			seg[5]=fre/100%10;
			seg[6]=fre/10%10;
			seg[7]=fre%10;
			break;
	}
	
}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD |= 0x05;			//设置定时器模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
}
void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1=1;
	EA=1;
}

void Timer1_Service() interrupt 3
{
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	if(++delay==1000)
	{
		TR0=0;
		delay=0;
		fre=TH0<<8|TL0;
		TH0=TL0=0;
		TR0=1;
	}
	Seg_Display(pos,seg[pos],pot[pos]);
}

void main()
{
	Timer1_Init();
	Timer0_Init();
	while(1)
	{
		Seg_Pro();
	
	}
}