#include <STC15F2K60S2.H>
#include "iic.h"
#include "onewire.h"
#include "uart.h"
#include "key.h"
#include <stdlib.h>
typedef unsigned char u8;
typedef unsigned int u16;

u8 uslow;
u8 urecv[10];
u8 urecvindex;
u8 usend[10];

void Uart_Pro()
{
	if(uslow) return ;
	uslow=1;
	if(urecv[0]==0xa0)
	{
		P2=P2&0x1f|0x80;
		P0=~P0;
		urecv[0]=0;
		urecvindex=0;
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

void Timer_Service() interrupt 1
{
	if(++uslow==200) uslow=0;
}

void UartServer() interrupt 4
{
	if(RI==1)
	{
		urecv[urecvindex]=SBUF;
		urecvindex++;
		RI=0;
	}
	
}
void main()
{
	P2=P2&0x1f|0x80;
	P0=0xff;
	Timer0_Init();
	UartInit();
	SendByte(0xa0);
	while(1)
	{
		Uart_Pro();
	}
}
	
