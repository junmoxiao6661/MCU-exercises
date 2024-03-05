#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
#include "ds1302.h"
#include "key.h"
#include "seg.h"
#include "led.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kold,kslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pos,segslow;
u8 mode;
u8 time[3]={0x10,0x00,0x00};
float t;
u8 tempt=23;
bit cmode=0;
u8 leddelay;
u8 relayflag;
bit l1flag=0;
u16 l1delay;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kold=kval;
	switch(kdown)
	{
		case 12:
			if(++mode==3) mode=0;
			break;
		case 13:
			cmode^=1;
			break;
		case 9:
			if(cmode==0&&mode==2)
			{
				if(++tempt>=100) tempt=99;
			}
			if(kval==9&&mode==1&&cmode==1)
			{
				mode=4;
			}
			break;
		case 8:
			if(cmode==0&&mode==2)
			{
				if(--tempt==9) tempt=10;
			}
			break;
	}
	
}
void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	t=rd_t();
	Read_Rtc(time);
	if(mode==4&&kval!=9)
			mode=1;
	switch(mode)
	{
		case 0:
			seg[0]=12;
			seg[1]=mode+1;
			seg[3]=10;
			seg[4]=10;
			seg[5]=(u8)t/10;
			seg[6]=(u8)t%10;
			seg[7]=(u16)(t*10)%10;
			pot[6]=1;
			break;
		case 1:
			seg[0]=12;
			seg[1]=mode+1;
			seg[3]=time[0]/16;
			seg[4]=time[0]%16;
			seg[5]=11;
			seg[6]=time[1]/16;
			seg[7]=time[1]%16;
			pot[6]=0;
			break;
		case 2:
			seg[0]=12;
			seg[1]=mode+1;
			seg[3]=10;
			seg[4]=10;
			seg[5]=10;
			seg[6]=tempt/10;
			seg[7]=tempt%10;
			pot[6]=0;
			break;
		case 4:
			seg[0]=12;
			seg[1]=2;
			seg[3]=time[1]/16;
			seg[4]=time[1]%16;
			seg[5]=11;
			seg[6]=time[2]/16;
			seg[7]=time[2]%16;
			pot[6]=0;
			break;
	}
}

void Led_Pro()
{
	if(t>(float)tempt)
		relayflag=1;
	else
	{
		led[2]=0;
		relayflag=0;
		leddelay=0;
	}
	if(time[1]==0x00&&time[2]==0x00)
		l1flag=1;
	led[1]=(cmode==0);
	
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
	Relay(relayflag);
	Beep(0);
	if(relayflag) 
	{
		if(++leddelay==100)
		{
			led[2]=~led[2];
			leddelay=0;
		}
	}
	if(l1flag)
	{
		led[0]=1;
		if(relayflag==0)
			Relay(l1flag);
		if(++l1delay==5000)
		{
			led[0]=0;
			l1flag=0;
			l1delay=0;
		}
	}
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
void Delay750ms()		//@11.0592MHz
{
	unsigned char i, j, k;

	i = 32;
	j = 133;
	k = 87;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

int main()
{
	sysinit();
	rd_t();
	Delay750ms();
	Timer0_Init();
	Set_Rtc(time);
	while(1)
	{
		Led_Pro();
		Key_Pro();
		Seg_Pro();
		
		
	}
}
