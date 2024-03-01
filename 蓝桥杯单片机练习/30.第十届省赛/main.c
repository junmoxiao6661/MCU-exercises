#include <STC15F2K60S2.H>
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kold,kdown,kslow;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u16 fre,fdelay;
u8 mode;
bit segmode=0;
bit ledmode=0;
bit vmode=0;
float v=2.0;
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
            if(++mode==2) mode=0;
            break;
		case 5:
			vmode^=1;
			break;
		case 6:
			ledmode^=1;
			break;
        case 7:
            segmode^=1;
            break;
    }
}
void Seg_Pro()
{
    u8 i=0;
    u16 tempf;
    if(segslow) return ;
    segslow=1;
	if(vmode)
		v=Ad_Read(0x03)/51.0;
	else
		v=2.0;
	Da_Write(51*v);
    if(segmode)
    {
		seg[7]=10;
		seg[6]=10;
		seg[5]=10;
		seg[4]=10;
		seg[3]=10;
		seg[0]=10;
		pot[5]=0;
    }
	else
	{
		switch (mode)
		{
			case 0 :
				tempf=fre;
				seg[0]=12;
				pot[5]=0;
				for(i=7;i>=3;i--)
				{
					if(tempf)
						seg[i]=tempf%10;
					else
						seg[i]=10;
					tempf/=10;
				}
				break;
			case 1:
				seg[0]=13;
				seg[3]=10;
				seg[4]=10;
				seg[5]=(u8)v;
				seg[6]=(u8)(v*10)%10;
				seg[7]=(u16)(v*100)%10;
				pot[5]=1;
				break;
		}
	}
}
void Led_Pro()
{
	if(ledmode)
	{
		led[0]=0;
		led[1]=0;
		led[2]=0;
		led[3]=0;
		led[4]=0;
	}
	else
	{
		led[0]=(mode==1);
		led[1]=(mode==0);
		led[2]=(v>=1.5&&v<2.5)||(v>=3.5);
		led[3]=(fre>=1000&&fre<5000)||(fre>=10000);
	}
}
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
    TMOD |= 0x05;
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
    ET0=1;
}

void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x20;				//设置定时初始值
	TH1 = 0xD1;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
    ET1=1;
    EA=1;
}

void Timer_Service() interrupt 3
{
    if(++kslow==10) kslow=0;
    if(++segslow==500) segslow=0;
    if(++pos==8) pos=0;
    Seg_Display(pos,seg[pos],pot[pos]);
    Led_Display(pos,led[pos]);
    if(++fdelay==1000)
    {
        TR0=0;
        fdelay=0;
        fre=TH0<<8|TL0;
        TH0=TL0=0;//别忘了清零
        TR0=1;
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
    Timer1_Init();
    Timer0_Init();
    while(1)
    {
        Key_Pro();
        Seg_Pro();
        Led_Pro();
    }
}
