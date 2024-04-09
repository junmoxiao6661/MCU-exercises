#include <STC15F2K60S2.H>
#include "iic.h"
#include "led.h"
#include "key.h"
#include "seg.h"
#include "uart.h"
#include <STRING.H>
#include <STDIO.H>
#include <STDLIB.H>
typedef unsigned char u8;
typedef unsigned int u16;

u8 kslow,kval,kold,kdown;
u8 segslow,pos;
u8 uslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode;
u8 recv[8];
u8 recvindex;
u8 send[14];
float v;
u8 tempv=65;
u16 delay;
bit flag=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kold=kval;
	switch(kdown)
	{
		case 12:
			if(++mode==2) mode=0;
			break;
		case 16:
			if(mode==1)
			{
				tempv+=5;
				if(tempv>90)
					tempv=90;
			}
			break;
		case 17:
			if(mode==1)
			{
				tempv-=5;
				if(tempv>200)
					tempv=0;
			}
			break;
	}
}

void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	v=AD_Read(0x43)/51.0;
	v=18.0*v;
	switch(mode)
	{
		case 0:
			seg[0]=11;
			seg[1]=1;
			seg[5]=(u8)v/10==0?10:(u8)v/10;
			seg[6]=(u8)v%10;
			seg[7]=(u16)(v*10)%10;
			pot[6]=1;
			break;
		case 1:
			seg[0]=11;
			seg[1]=2;
			seg[5]=10;
			seg[6]=tempv/10==0?10:tempv/10;
			seg[7]=tempv%10;
			pot[6]=0;
			break;
	}
}

void Led_Pro()
{
	led[0]=(mode==0);
	led[1]=(mode==1);
	if(v>tempv) 
		flag=1;
	else
	{
		flag=0;
		delay=0;
		led[7]=0;
	}
	if(delay==100)
	{
		delay=0;
		led[7]=~led[7];
	}
	
	
}

void Uart_Pro()
{
	if(mode==0&&strcmp(recv,"Return")==0&&recvindex)
	{
		sprintf(send,"Noises:%.1fdB",v);
		SendString(send);
		recvindex=0;
		memset(recv,0,8*sizeof(u8));
	}
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
	if(flag) ++delay;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
}
void Uart_Server() interrupt 4
{
	
		if(RI==1)
		{
			if(mode==0)
			{
				recv[recvindex]=SBUF;
				recvindex++;
			}
			RI=0;
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
	AD_Read(0x43);
	sysinit();
	
	Timer0_Init();
	UartInit();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		Uart_Pro();
	}
}