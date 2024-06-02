#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
#include "seg.h"
#include "led.h"
#include "key.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kslow,kold,kup;
u8 pos,segslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pot[8]={0,0,0,0,0,0,0,0};
u16 fre=0,fredelay=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kup=~kval&(kval^kold);
	
}

void Seg_Pro()
{
	u8 i;
	u16 j;
	if(segslow) return ;
	segslow=1;
	
}

void Led_Pro()
{
	
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
// IE2|0x04
void Timer1_Service() interrupt 3
{
	if(++kslow==10) kslow=0;
	if(++segslow==200) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	
}
void sysinit()
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2=P2&0x1f;
}

void main()
{
	sysinit();
	Timer1_Init();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}