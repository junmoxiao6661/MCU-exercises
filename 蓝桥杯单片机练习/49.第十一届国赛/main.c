#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"
#include "key.h"
#include "led.h"
#include "seg.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pos;
u16 segslow;
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pot[8]={0,0,0,0,0,0,0,0};
bit mode=0;
u8 dmode=0;
u8 pmode=0;
u8 time[3]={0x16,0x59,0x50};
float t,light;
u8 phour=17,seghour=17;
u8 pt=25,segt=25;
u8 pled=4,segled=4;
bit led1=0;
bit lightflag=0;
u16 delay3s=0,delay3s1=0;
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
			if(mode==0)
			{				
				mode=1,pmode=0;
			}
			else if(mode==1)
			{
				mode=0,dmode=0;
				led[pled-1]=0;
				phour=seghour,pt=segt,pled=segled;
			}
			break;
		case 5:
			if(mode==0)
			{
				if(++dmode==3) dmode=0;
			}
			else
			{
				if(++pmode==3) pmode=0;
			}
			break;
		case 8:
			if(mode==1)
			{
				if(pmode==0)
				{
					if(--seghour==255)
					{
						seghour=0;
					}
				}
				else if(pmode==1)
				{
					if(--segt==255)
					{
						segt=0;
					}
				}
				else if(pmode==2)
				{
					if(--segled==3)
					{
						segled=4;
					}
				}
			}
			break;
		case 9:
			if(mode==1)
			{
				if(pmode==0)
				{
					if(++seghour==24)
					{
						seghour=23;
					}
				}
				else if(pmode==1)
				{
					if(++segt==100)
					{
						segt=99;
					}
				}
				else if(pmode==2)
				{
					if(++segled==9)
					{
						segled=8;
					}
				}
			}
			break;
	}
	
}

void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	Read_Rtc(time);
	t=rd_t();
	light=AD_Read(0x01)/51.0;
	if(led1==0&&(time[0]/16*10+time[0]%16>=phour))
	{
		led1=1;
	}
	if((time[0]/16*10+time[0]%16<phour)&&led1==1)
	{
		led1=0;
	}
	if(led1==1&&(time[0]/16*10+time[0]%16==8))
	{
		led1=0;
	}
	if(mode==0)
	{
		pot[2]=pot[6]=0;
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
				seg[1]=seg[2]=seg[3]=seg[4]=10;
				seg[5]=(u8)(t)/10;
				seg[6]=(u8)(t)%10;
				pot[6]=1;
				seg[7]=(u16)(t*10)%10;
				break;
			case 2:
				seg[0]=13;
				seg[1]=seg[5]=seg[6]=10;
				pot[2]=1;
				seg[2]=(u8)light;
				seg[3]=(u8)(light*10)%10;
				seg[4]=(u16)(light*100)%10;
				seg[7]=light<3?0:1;
				break;
		}
	}
	else
	{
		pot[2]=pot[6]=0;
		seg[0]=14;
		seg[2]=seg[3]=seg[4]=seg[5]=seg[6]=10;
		switch(pmode)
		{
			case 0:
				seg[1]=1;
				seg[6]=seghour/10;
				seg[7]=seghour%10;
				break;
			case 1:
				seg[1]=2;
				seg[6]=segt/10;
				seg[7]=segt%10;
				break;
			case 2:
				seg[1]=3;
				seg[7]=segled;
				break;
		}
	}
}


void Led_Pro()
{
	led[0]=led1;
	led[1]=t<pt;
	led[2]=lightflag;
	led[pled-1]=light<3;
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
	if(++segslow==200) segslow=0;
	if(++pos==8) pos=0;
	if(light<3) 
	{
		if(++delay3s==3000)
		{
			lightflag=1;
		}
		delay3s1=0;
	}
	else
	{
		if(++delay3s1==3000)
		{
			lightflag=0;
		}
		delay3s=0;
	}
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
}
void sysinit()
{
	P0=0xff;
	P2=P2&0x11f|0x80;
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
	Set_Rtc(time);
	AD_Read(0x01);
	rd_t();
	Delay750ms();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}