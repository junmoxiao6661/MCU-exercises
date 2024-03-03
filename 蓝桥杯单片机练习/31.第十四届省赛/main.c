#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
#include "ds1302.h"
#include "key.h"
#include "seg.h"
#include "led.h"

typedef unsigned char u8;
typedef unsigned int u16;
u8 kval,kdown,kslow,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u16 fre,fredelay;
u8 mode,backmode,tempmode;
u8 time[3]={0x18,0x20,0x00};
float t,maxt,avt,tempt;
float rh,maxrh,avrh,temprh;
u16 sumt,sumrh;
u8 cnt,hour,min;
u8 rhcnt;
u8 pt=30;
u16 s9delay;
bit flag=0;
u8 light;
bit lightflag=0;
bit ledflag;
u16 lightdelay;
u8 leddelay;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kold=kval;
	switch(kdown)
	{
		case 4:
			if(++mode==3) mode=0;
			break;
		case 5:
			if(mode==1)
			{
				if(++backmode==3) backmode=0;
			}
			break;
		case 8:
			if(mode==2)
			{
				if(++pt>=100) pt=99;
			}
			break;
		case 9:
			if(mode==2)
			{
				if(--pt==255) pt=0;
			}
			flag=1;
			break;
	}

}


void Timer0_Init(void)		//100微秒@12.000MHz
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
void rd_rh()
{
	if(fre>=200&&fre<=2000)
	{
		rh=0.0445*fre+1.1111;
	}
}
void Trigger()
{
	light=Ad_Read(0x01);
	if(light<=150&&lightflag==0&&cnt<99)
	{
		t=rd_t();
		rd_rh();
		cnt++;
		lightflag=1;
		tempmode=mode;
		hour=time[0];
		min=time[1];
		tempt=t;
		if(maxt<tempt)
			maxt=tempt;
		sumt+=tempt;
		avt=sumt/(float)cnt;
		if(fre>=200&&fre<=2000)
		{
			rhcnt++;
			temprh=rh;
			if(maxrh<temprh)
				maxrh=temprh;
			sumrh+=temprh;
			avrh=sumrh/(float)rhcnt;
		}
	}
}

void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	Trigger();
	Read_Rtc(time);
	switch(mode)
	{
		case 0:
			seg[0]=time[0]/16;// 16进制
			seg[1]=time[0]%16;
			seg[2]=11;
			seg[3]=time[1]/16;
			seg[4]=time[1]%16;
			seg[5]=11;
			seg[6]=time[2]/16;
			seg[7]=time[2]%16;
			pot[6]=0;
			break;
		case 1:
			if(backmode==0)
			{
				seg[0]=12;// 16进制
				seg[1]=10;
				if(cnt)
				{
					seg[2]=(u8)maxt/10;
					seg[3]=(u8)maxt%10;
					seg[4]=11;
					seg[5]=(u8)avt/10;
					seg[6]=(u8)avt%10;
					pot[6]=1;
					seg[7]=(u16)(avt*10)%10;
				}
				else
				{
					seg[2]=10;
					seg[3]=10;
					seg[4]=10;
					seg[5]=10;
					seg[6]=10;
					pot[6]=0;
					seg[7]=10;
				}
				
			}
			else if(backmode==1)
			{
				seg[0]=13;// 16进制
				seg[1]=10;
				if(cnt)
				{
					seg[2]=(u8)maxrh/10;
					seg[3]=(u8)maxrh%10;
					seg[4]=11;		
					seg[5]=(u8)avrh/10;
					seg[6]=(u8)avrh%10;
					pot[6]=1;
					seg[7]=(u16)(avrh*10)%10;
				}
				else
				{
					seg[2]=10;
					seg[3]=10;
					seg[4]=10;
					seg[5]=10;
					seg[6]=10;
					pot[6]=0;
					seg[7]=10;
				}
			}
			else if(backmode==2)
			{
				seg[0]=14;
				seg[1]=cnt/10;
				seg[2]=cnt%10;
				pot[6]=0;
				if(cnt)
				{
					seg[3]=hour/16;
					seg[4]=hour%16;
					seg[5]=11;
					seg[6]=min/16;
					seg[7]=min%16;
				}
				else
				{
					seg[3]=10;
					seg[4]=10;
					seg[5]=10;
					seg[6]=10;
					seg[7]=10;
				}
			}
			break;
		case 2:
			seg[0]=15;
			seg[1]=10;
			seg[2]=10;
			seg[3]=10;
			seg[4]=10;
			seg[5]=10;
			seg[6]=pt/10;
			seg[7]=pt%10;
			pot[6]=0;
			break;
		case 4:
			seg[0]=16;
			seg[1]=10;
			seg[2]=10;
			pot[6]=0;
			seg[3]=(u8)t/10;
			seg[4]=(u8)t%10;
			seg[5]=11;
			if(fre>=200&&fre<=2000)
			{
				seg[6]=(u8)rh/10;
				seg[7]=(u8)rh%10;
			}
			else
			{
				seg[6]=17;
				seg[7]=17;
			}
			break;
	}
}

void Led_Pro()
{
	led[0]=(mode==0);
	led[1]=(mode==1);
	led[2]=(mode==4);
	if(tempt>pt) 
	{
		ledflag=1;
		if(leddelay==100)
		{
			led[3]=~led[3];
			leddelay=0;
		}
	}
	else 
	{
		led[3]=0;
		ledflag=0;
		leddelay=0;
	}
	led[4]=(fre<200||fre>2000)&&(mode==4);
	led[5]=cnt>=2&&t>tempt&&rh>temprh;
}
void Timer_Service() interrupt 3
{
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(++fredelay==1000)
	{
		TR0=0;
		fredelay=0;
		fre=(TH0<<8)|TL0;
		TL0=TH0=0;
		TR0=1;
	}
	if(flag&&kval==9)
	{
		if(++s9delay==2000)
		{
			s9delay=0;
			flag=0;
			cnt=0;
			rhcnt=0;
		}
	}
	else
	{
		flag=0;
		s9delay=0;
	}
	if(lightflag)
	{
		mode=4;
		if(++lightdelay==3000)
		{
			lightflag=0;
			lightdelay=0;
			mode=tempmode;
		}
	}
	if(ledflag) ++leddelay;
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
	rd_t();
	Ad_Read(0x01);
	Timer1_Init();
	Timer0_Init();	
	Set_Rtc(time);
	while(1)
	{
		Led_Pro();
		Key_Pro();
		Seg_Pro();
		
	}
}