#include <STC15F2K60S2.H>
#include "onewire.h"
#include "ds1302.h"
#include "iic.h"
#include "led.h"
#include "key.h"
#include "seg.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pot[8]={0,0,0,0,0,0,0,0};
float t;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kold=kval;
}
void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	t=AD_Read(0x03)/51.0;
	seg[5]=(u8)t;
	seg[6]=(u8)(t*10)%10;
	seg[7]=(u16)(t*100)%10;
	pot[5]=1;
	
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
void main()
{
	sysinit();
	AD_Read(0x03);
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}