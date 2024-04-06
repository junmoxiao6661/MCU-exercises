#include <STC15F2K60S2.H>
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"
#include "led.h"
#include "key.h"
#include "seg.h"
#include "ultrasound.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kslow,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode;
u8 time[3]={0x23,0x59,0x55};
float t,v;
u16 dis;
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
            if(++mode==4) mode=0;
            break;
    }
}

void Seg_Pro()
{
    if(segslow) return ;
    segslow=1;
    Read_Rtc(time);
    t=Read_T();
    v=AD_Read(0x03)/51.0;
    dis=Wave_Data();
    switch(mode)
    {
        case 0:
            seg[0]=1;
            seg[2]=time[0]/16;
            seg[3]=time[0]%16;
            seg[4]=time[1]/16;
            seg[5]=time[1]%16;
            seg[6]=time[2]/16;
            seg[7]=time[2]%16;
            pot[6]=0;
            break;
        case 1:
            seg[0]=2;
            seg[2]=10;
            seg[3]=10;
            seg[4]=10;
            seg[5]=10;
            seg[6]=(u8)t/10;
            seg[7]=(u8)t%10;
            pot[6]=0;
            break;
        case 2:
            seg[0]=3;
            seg[2]=10;
            seg[3]=10;
            seg[4]=10;
            seg[5]=10;
            seg[6]=(u8)v;
            seg[7]=(u8)(v*10)%10;
            pot[6]=1;
            break;
        case 3:
            seg[0]=4;
            seg[2]=10;
            seg[3]=10;
            seg[4]=10;
            seg[5]=dis/100==0?10:dis/100;
            seg[6]=dis/100==0&&dis/10%10==0?10:dis/10%10;
            seg[7]=dis%10;
            pot[6]=0;
            break;
    }

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
    Read_T();
    Delay750ms();
    AD_Read(0x03);
    Set_Rtc(time);
    Timer0_Init();
    while(1)
    {
        Key_Pro();
        Seg_Pro();
    }
}