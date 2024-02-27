#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kup,kdown,kold,kslow;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 tmax=30,tmin=20;
u8 t1=30,t2=20;
u8 mode=0;
bit tmode=0;
float T=0.0;
u8 tempt=0;
bit wrong=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();//别忘了
	kdown=kval&(kold^kval);
	kold=kval;
	switch(kdown)
	{
		case 4:
			if(++mode==2) 
			{
				if(tmax>=tmin)
				{
					t1=tmax;
					t2=tmin;
					wrong=0;
				}
				else
				{
					tmax=t1;
					tmin=t2;
					wrong=0;
				}
				mode=0;
			}
			if(mode==1) tmode=0;
			break;
		case 5 :
			tmode^=1;
			break;
		case 6:
			if(mode==1)
			{
				if(tmode==0)
				{
					++tmin;
					if(tmin>tmax) wrong=1;
					else if(tmin==100) tmin=99,wrong=1;
					else wrong=0;
				}
				else
				{
					++tmax;
					if(tmax==100) tmax=99,wrong=1;
					else wrong=0;
				}
			}
			break;
		case 7:
			if(mode==1)
			{
				if(tmode==0)
				{
					--tmin;
					if(tmin==255) tmin=0,wrong=1;
					else wrong=0;
				}
				else
				{
					--tmax;
					if(tmax<tmin) wrong=1;
					else if(tmax==255) tmax=0,wrong=1;
					else wrong=0;
				}
			}
			break;
	}
}
void Seg_Pro()
{
	if(segslow) return;
	segslow=1;
	T=rd_t();
	tempt=T;
	switch(mode)
	{
		case 0 :
			seg[0]=12;
			seg[3]=10;
			seg[4]=10;
			seg[6]=tempt/10;
			seg[7]=tempt%10;
			break;
		case 1 :
			seg[0]=13;
			seg[3]=tmax/10;
			seg[4]=tmax%10;
			seg[6]=tmin/10;
			seg[7]=tmin%10;
			break;
			
	}
}
void Led_Pro()
{
	led[0]=(tempt>tmax);
	led[1]=(tempt>=tmin&&tempt<=tmax);
	led[2]=(tempt<tmin);
	led[3]=wrong;
}
void DA()
{
	if(tempt>tmax)
		Da_Write(4*51);
	else if(tempt>=tmin&&tempt<=tmax)
		Da_Write(3*51);
	else 
		Da_Write(2*51);
		
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

void main()
{
	sysinit();
	rd_t();
	Delay750ms();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		DA();
	}
}