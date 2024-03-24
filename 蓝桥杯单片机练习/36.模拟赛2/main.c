#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
#include "uart.h"
#include "key.h"
#include "led.h"
#include "seg.h"
#include <STDIO.H>
typedef unsigned char u8;
typedef unsigned int u16;
u8 kval,kdown,kold,kslow;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 uslow;
u8 urecv[10];
u8 urecvindex;
u8 usend[10];
u8 mode;
u8 delay100;
float t;
float v;
char str1[15];
char str2[15];
bit flag=1;
bit ledflag;
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
			flag=0;
			break;
		case 5:
			if(!flag)
				flag=1;
			break;
		case 12:
			if(mode==0)
			{
				str1[5]=(u8)t/10+'0';
				str1[6]=(u8)t%10+'0';
				str1[7]='.';
				str1[8]=(u16)(t*10)%10+'0';
				str1[9]='C';
				//sprintf(str1,"TEMP:%.1f°C",t);
				SendString(str1);
			}
			else
			{
				str2[8]=(u8)v%10+'0';
				str2[9]='.';
				str2[10]=(u8)(v*10)%10+'0';
				str2[11]=(u16)(v*100)%10+'0';
				str2[12]='V';
				//sprintf(str2,"Voltage:%.2f",v);
				SendString(str2);
			}
			break;
	}
}
void Seg_Pro()
{
	if(segslow) return;
	segslow=1;
	t=rd_t();
	v=Ad_Read(0x03)/51.0;
	switch(mode)
	{
		case 0:
			seg[0]=11;
			seg[1]=1;
			seg[5]=(u8)t/10;
			seg[6]=(u8)t%10;
			seg[7]=(u16)(t*10)%10;
			pot[6]=1;
			pot[5]=0;
			break;
		case 1:
			seg[0]=11;
			seg[1]=2;
			seg[5]=(u8)v%10;
			seg[6]=(u8)(v*10)%10;
			seg[7]=(u16)(v*100)%10;
			pot[6]=0;
			pot[5]=1;
			break;
			
	}
}

void Led_Pro()
{
	if(t>=28)
		Relay(1);
	else
		Relay(0);
	if(v>3.6)
		Beep(1);
	else
		Beep(0);
	led[0]=(mode==0);
	led[1]=(mode==1);
	if(flag==1)
	{
		led[2]=0;
		delay100=0;
	}
}
void Uart_Pro()
{
	if(uslow) return ;
	uslow=1;
	
	if(urecv[0]=='A')
		mode=0;
	if(urecv[0]=='B')
		mode=1;
	
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
	if(++uslow==200) uslow=0;
	
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(!flag)
	{	
		if(++delay100==100)
		{
			led[2]=~led[2];
			delay100=0;
		}
	}
}

void UartServer() interrupt 4
{
	if(RI==1)
	{
		if(flag)
			urecv[urecvindex]=SBUF;
		//urecvindex++;
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
	Ad_Read(0x03);
	Delay750ms();
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
	
