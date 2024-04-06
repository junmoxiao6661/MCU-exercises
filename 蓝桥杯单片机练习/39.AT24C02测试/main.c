#include <STC15F2K60S2.H>
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"
#include <stdio.h>

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 time[3]={0x00,0x00,0x00};
u8 mode;
u8 arr[4];
u8 arr_pos;
u8 code key_map[19]={0,0,0,0,0,0,1,0,8,5,2,0,9,6,3,0,10,7,4};
u8 start_t[2];
u8 date[2];
u8 high;
u8 low;
u16 num;
u8 x,y;
void Seg_Pro()
{

	if(segslow) return ;
	segslow=1;
	seg[4]=num/1000;
	seg[5]=num/100%10;
	seg[6]=num/10%10;
	seg[7]=num%10;
	
}
void Led_Pro()
{
	
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
	if(++segslow==500) return ;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
}

void sysinit()
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2=P2&0x1f;
}
void Delay10ms()		//@12.000MHz
{
	unsigned char i, j;

	i = 117;
	j = 184;
	do
	{
		while (--j);
	} while (--i);
}

void main()
{
	
	high=6666/256;
	low=6666%256;
	Write_EEPROM(&high,0,1);
	//Delay10ms();
	Write_EEPROM(&low,1,1);
	
	Read_EEPROM(&x,0,1);
	Read_EEPROM(&y,1,1);
	
	num=(x<<8)|y;
	sysinit();
	Timer0_Init();
	while(1)
	{
		Seg_Pro();
		Led_Pro();
	}
}
