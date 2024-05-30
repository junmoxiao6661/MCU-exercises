#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "iic.h"
#include "seg.h"
#include "led.h"
#include "key.h"
#include "ultrasound.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 pos;
u16 segslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pot[8]={0,0,0,0,0,0,0,0};
bit mode=0;
u8 dmode=0;
u8 tmode=0;
u8 drmode=0;
u8 time[3]={0x20,0x20,0x01};
bit dismode=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kold=kval;
	switch(kdown)
	{
		case 4:
			mode^=1;
			break;
		case 5:
			if(mode==0)
				{if(++dmode==3) dmode=0;}
			else if(mode==1)
				{if(++tmode==2) tmode=0;}
			break;
		case 8:
			if(mode==0&&dmode==1) dismode^=1;
			if(mode==0&&dmode==2) 
				{if(++drmode==3) drmode=0;}
			break;
		case 9:
			
			break;
	}
	
}
void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	Read_Rtc(time);
	if(mode==0)
	{
		switch(dmode)
		{
			case 0:
				seg[0]=time[0]/16;
				seg[1]=time[0]%16;
				seg[2]=11;
				seg[3]=time[1]/16;
				seg[4]=time[1]%16;
				seg[5]=11;
				seg[6]=time[2]/16;
				seg[7]=time[2]%16;
				break;
			case 1:
				seg[0]=12;
				seg[1]=(dismode==0?13:14);
				seg[2]=seg[3]=seg[4]=10;
				
				break;
			case 2:
				seg[0]=15;
				seg[2]=seg[3]=10;
				switch(drmode)
				{
					case 0:
						seg[1]=16;
						break;
					case 1:
						seg[1]=11;
						break;
					case 2:
						seg[1]=17;
						break;
				}
				break;
		}
	}
	else
	{
		seg[0]=18;
		seg[2]=seg[3]=seg[4]=seg[5]=10;
		switch(tmode)
		{
			case 0:
				seg[1]=1;
				break;
			case 1:
				seg[1]=2;
				break;
		}
	}
}
void Led_Pro()
{
	
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
void Timer_Service() interrupt 1
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
	P2=P2&0x1f;
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2=P2&0x1f;
}
void main()
{
	sysinit();
	Set_Rtc(time);
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}