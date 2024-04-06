#include <STC15F2K60S2.H>
#include "ultrasound.h"
#include "key.h"
#include "seg.h"
#include "led.h"
#include "uart.h"

typedef unsigned char u8;
typedef unsigned int u16;
u8 kval,kslow,kold,kdown;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode;
u16 dis,pdis=30;
u8 uslow;
u8 recv[10];
u8 recvindex;
u8 usend[8];
u8 str[20]="Distance:";
bit flag=0;
u8 delay;
//123
void myitoa()
{
	if(dis>=100)
	{
		str[9]=dis/100+'0';
		str[10]=dis/10%10+'0';
		str[11]=dis%10+'0';
		str[12]='c';
		str[13]='m';
		str[14]='\0';
	}
	else if(dis>=10&&dis<=99)
	{
		str[9]=dis/10+'0';
		str[10]=dis%10+'0';
		str[11]='c';
		str[12]='m';
		str[13]='\0';
	}
	else
	{
		str[9]=dis+'0';
		str[10]='c';
		str[11]='m';
		str[12]='\0';
	}
}
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
			if(++mode==2) mode=0;
			break;
		case 8:
			if(mode==0) pdis=dis;
			break;
		case 12:
			if(mode==1)
			{
				pdis+=10;
				if(pdis>999)
					pdis=999;
			}
			break;
		case 16:
			if(mode==1)
			{
				if(pdis<=10)
					pdis=0;
				else
					pdis-=10;
				
			}
			break;
		case 9:
			myitoa();
			SendString(str);
			break;
			
	}
}
void Seg_Pro()
{
	if(segslow) return;
	segslow=1;
	dis=Ut_Wave_Data();
	switch(mode)
	{
		case 0:
			seg[0]=11;
			seg[1]=1;
			seg[5]=dis/100==0?10:dis/100;
			seg[6]=dis/100==0&&dis/10%10==0?10:dis/10%10;
			seg[7]=dis%10;
			break;
		case 1:
			seg[0]=11;
			seg[1]=2;
			seg[5]=pdis/100==0?10:pdis/100;
			seg[6]=pdis/100==0&&pdis/10%10==0?10:pdis/10%10;
			seg[7]=pdis%10;
			break;
	}
}
void Led_Pro()
{
	led[0]=(mode==0);
	led[1]=(mode==1);
	if(dis>pdis)
		flag^=1;
	else
	{
		flag^=1;
		led[2]=0;
		delay=0;
	}
	//led[0]=1;
}
void Uart_Pro()
{
	if(uslow) return ;
	uslow=1;
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

void Timer1_Service() interrupt 1
{
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	if(++uslow==200) uslow=0;
	if(flag)
	{
		if(++delay==200)
		{
			led[2]=~led[2];
			delay=0;
		}
	}
	Seg_Display(pos,seg[pos],pot[pos]);
	Led(pos,led[pos]);
}
void Uart_Server() interrupt 4
{
	if(RI==1)
	{
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
	sysinit();
	
	Uart_Init();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		Uart_Pro();
	}
}