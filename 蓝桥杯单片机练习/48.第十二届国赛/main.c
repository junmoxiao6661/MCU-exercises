#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "iic.h"
#include "seg.h"
#include "led.h"
#include "key.h"
#include "ultrasound.h"
#include "STDIO.H"

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
u8 ptime=2,pdis=20;
u8 seg_time=2,seg_dis=20;
u16 rd1,oldrd1=255;
u16 dis=0;
bit light=1;
bit trigger=0,flag=0;
u16 maxdis=0,mindis=999,sumdis=0;
float averagedis=0.0;
u8 discnt;
u16 alarmcnt=0;
bit alarm=0;
u16 is1s=0;


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
				mode=1,tmode=0;
			else
				mode=0,dmode=0,ptime=seg_time,pdis=seg_dis;
			break;
		case 5:
			if(mode==0)
			{
				if(++dmode==3) dmode=0;
				if(dmode==2) drmode=0;
			}
			else if(mode==1)
				{if(++tmode==2) tmode=0;}
			break;
		case 8:
			if(mode==0&&dmode==1) dismode^=1;
			if(mode==0&&dmode==2) 
				{if(++drmode==3) drmode=0;}
			break;
		case 9:
			if(mode==1)
			{
				if(tmode==0)
					{if(++seg_time==10) seg_time=2;}
				else
				{
					seg_dis+=10;
					if(seg_dis>=90)
						seg_dis=10;
				}
			}
			break;
	}
	
}

void Seg_Pro()
{
	u8 temp;
	u16 temp1;
	if(segslow) return ;
	segslow=1;
	Read_Rtc(time);
	rd1=AD_Read(0x01);
	if(rd1>150)
	{
		light=1;
		flag=1;
	}
	else if(rd1<=150&&flag)
	{
		light=0;
		trigger=1;
	}
	if(dismode==0)
	{
		if(trigger)
		{
			dis=Wave_Data();
			trigger=0;
			flag=0;
			discnt++;
			sumdis+=dis;
			if(dis>maxdis)
				maxdis=dis;
			if(dis<mindis)
				mindis=dis;
			averagedis=sumdis/(float)discnt;
		}
	}
	else
	{
		temp=(time[2]/16)*10+time[2]%16;
		if(temp%ptime==0&&is1s>=800)
		{
			is1s=0;
			dis=Wave_Data();
			discnt++;
			sumdis+=dis;
			if(dis>maxdis)
				maxdis=dis;
			if(dis<mindis)
				mindis=dis;
			averagedis=sumdis/(float)discnt;
			if(dis>=pdis-5&&dis<=pdis+5)
				alarmcnt++;
			else
			{
				alarmcnt=0;
				alarm=0;
			}
			if(alarmcnt>=3)
				alarm=1;
		}	
		
	}
	if(mode==0)
	{
		pot[6]=0;
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
				seg[5]=dis/100==0?10:dis/100;
				seg[6]=(dis/100==0&&dis/10%10==0)?10:dis/10%10;
				seg[7]=dis%10;
				break;
			case 2:
				seg[0]=15;
				seg[2]=seg[3]=seg[4]=10;
				switch(drmode)
				{
					case 0:
						seg[1]=16;
						seg[5]=maxdis/100==0?10:maxdis/100;
						seg[6]=(maxdis/100==0&&maxdis/10%10==0)?10:maxdis/10%10;
						seg[7]=maxdis%10;
						break;
					case 1:
						seg[1]=11;
						pot[6]=1;
						seg[7]=(u16)(averagedis*10)%10;
						temp1=(u16)averagedis;
						seg[6]=temp1%10;
						temp1/=10;
						temp=5;
						while(temp1!=0)
						{
							seg[temp]=temp1%10;
							temp1/=10;
							temp--;
						}
						break;
					case 2:
						seg[1]=17;
						seg[5]=mindis/100==0?10:mindis/100;
						seg[6]=(mindis/100==0&&mindis/10%10==0)?10:mindis/10%10;
						seg[7]=mindis%10;
						break;
				}
				break;
		}
	}
	else
	{
		seg[0]=18;
		seg[2]=seg[3]=seg[4]=seg[5]=10;
		pot[6]=0;
		switch(tmode)
		{
			case 0:
				seg[1]=1;
				seg[6]=seg_time/10;
				seg[7]=seg_time%10;
				break;
			case 1:
				seg[1]=2;
				seg[6]=seg_dis/10;
				seg[7]=seg_dis%10;
				break;
		}
	}
}
void Led_Pro()
{
	if(mode==0)
	{
		led[0]=dmode==0;
		led[1]=dmode==1;
		led[2]=dmode==2;
	}
	led[3]=dismode==0;
	led[4]=alarm;
	led[5]=light;
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
	++is1s;
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
	Wave_Data();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}