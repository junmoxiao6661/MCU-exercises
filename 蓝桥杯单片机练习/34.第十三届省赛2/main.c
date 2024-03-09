#include <STC15F2K60S2.H>
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"
#include "ultrasonic.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 dis;
u8 mode;
float v;
float vh=4.5,vl=0.5;
float tvh=4.5,tvl=0.5;
bit disflag;
bit tmode;
u8 disdelay;
void Key_Pro()
{
	if(kslow) return;
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
			if(mode==2) tmode^=1;
			break;
		case 6:
			if(mode==2&&tmode==0)
			{
				tvh+=0.5;
				if(tvh>5) tvh=0.5;
			}
			if(mode==2&&tmode==1)
			{
				tvl+=0.5;
				if(tvl>5) tvl=0.5;
				if(tvl>=tvh) tvl=tvh;
			}
			break;
		case 7:
			if(mode==2&&tmode==0)
			{
				tvh-=0.5;
				if(tvh<0.5) tvh=5.0;
				if(tvh<=tvl) tvh=tvl;
			}
			if(mode==2&&tmode==1)
			{
				tvl-=0.5;
				if(tvl<0.5) tvl=5.0;
			}
			break;
				
	}
}
void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	v=Ad_Read(0x03)/51.0;
	if(mode!=2)
	{
		vh=tvh;
		vl=tvl;
	}
	if(v>vl&&v<vh)
	{
		dis=Wave_Read();
		disflag=1;
		if(dis<=20)
			Da_Write(51);
		else if(dis>20&&dis<80)
			Da_Write(51*(0.0667*(float)dis-0.3333));
		else
			Da_Write(51*5);
	}
	else
	{
		disflag=0;
		disdelay=0;
		led[7]=0;
		Da_Write(0);
	}
	switch(mode)
	{
		case 0:
			seg[0]=12;
			seg[3]=10;
			seg[4]=10;
			seg[5]=(u8)v;
			seg[6]=(u8)(v*10)%10;
			seg[7]=(u16)(v*100)%10;
			pot[5]=1;
			pot[3]=0;
			pot[6]=0;
			break;
		case 2:
			seg[0]=13;
			seg[3]=(u8)tvh;
			seg[4]=(u8)(tvh*10)%10;
			seg[5]=10;
			pot[3]=1;
			seg[6]=(u8)tvl;
			seg[7]=(u8)(tvl*10)%10;
			pot[6]=1;
			pot[5]=0;
			break;
		case 1:
			seg[0]=14;
			seg[3]=10;
			seg[4]=10;
			if(disflag)
			{
				seg[5]=dis/100==0?10:dis/100;
				seg[6]=dis/100==0&&dis/10%10==0?10:dis/10%10;
				seg[7]=dis%10;
			}
			else
			{
				seg[5]=15;
				seg[6]=15;
				seg[7]=15;
			}
			pot[3]=0;
			pot[6]=0;
			pot[5]=0;
			break;
	}
}

void Led_Pro()
{
	led[0]=(mode==0);
	led[1]=(mode==1);
	led[2]=(mode==2);
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


void Timer0_Servive() interrupt 1
{
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(disflag)
	{
		if(++disdelay==100)
		{
			led[7]=~led[7];
			disdelay=0;
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
void main()
{
	sysinit();
	Ad_Read(0x03);
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		
	}
}
