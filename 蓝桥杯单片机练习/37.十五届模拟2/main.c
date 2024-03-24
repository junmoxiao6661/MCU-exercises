#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
#include "key.h"
#include "seg.h"
#include "led.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode;
float t;
float ad;
u8 lux;
bit flag=0;
void Key_Pro()
{
	if(kslow) return;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kold=kval;
	
	switch(kdown)
	{
		case 4:
			if(flag==0)
				{if(++mode==2) mode=0;}
			break;
		case 5:
			flag^=1;
			break;
	}
	
}

void DA()
{
	if(mode==0)
	{
		if(t<=10)
			ad=1;
		else if(t>=40)
			ad=5;
		else
			ad=0.1333*t-0.3333;
	}
	else if(mode==1)
	{
		if(lux<=10)
			ad=1;
		else if(lux>=240)
			ad=5;
		else
			ad=0.0174*lux+0.8261;
	}
}
void Seg_Pro()
{
	if(segslow) return;
	segslow=1;
	t=rd_t();
	lux=Ad_Read(0x41);
	if(mode==0&&!flag)
	{
		seg[0]=1;
		seg[5]=(u8)t/10;
		seg[6]=(u8)t%10;
		pot[6]=1;
		seg[7]=(u16)(t*10)%10;
	}
	else if(mode==1&&!flag)
	{
		seg[0]=2;
		seg[5]=lux/100==0?10:lux/100;
		seg[6]=lux/10%10==0&&lux/100==0?10:lux/10%10;
		pot[6]=0;
		seg[7]=lux%10;
	}
	else if(flag)
	{
		DA();
		seg[0]=11;
		seg[5]=10;
		seg[6]=(u8)ad%10;
		pot[6]=1;
		seg[7]=(u16)(ad*10)%10;
		Da_Write((u8)(ad*51.0));
	}
	
}

void Led_Pro()
{
	led[0]=(mode==0);
	led[1]=(mode==1);
}
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=1;
	EA=1;
}

void Timer0_Service() interrupt 1
{
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
}
void sysinit()
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2=P2&0x1f;
	
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2=P2&0x1f;
	
	
}
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;

	i = 35;
	j = 51;
	k = 182;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void main()
{
	sysinit();
	rd_t();
	Delay750ms();
	Ad_Read(0x41);
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}