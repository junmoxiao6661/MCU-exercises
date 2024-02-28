#include <STC15F2K60S2.H>
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kup,kold,kslow;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode=0;
u8 count=0;
u8 flag;
u8 save;
u8 wrong;
u16 delay5s=0;
float v,vtemp;
float vp=3.0;
bit led1=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kold=kval;
	switch(kdown)
	{
		case 12:
			if(++mode==3) mode=0;
			if(mode==2)  
			{
				save=vp*10;
				EEPROM_Write(&save,0,1);
			}
			break;
		case 13:
			if(mode==2) count=0,wrong=0;
			else wrong++;
			break;
		case 16:
			if(mode==1)
			{
				wrong=0;
				vp+=0.5;
				if(vp>5.0)
					vp=0;
			}
			else wrong++;
			break;
		case 17:
			if(mode==1)
			{
				wrong=0;
				vp-=0.5;
				if(vp<0)
					vp=5.0;
			}
			else wrong++;
			break;
	}
}
void vcount()
{
	switch(flag)
	{
		case 0 :
			if(v>vp)
				flag=1;
			break;
		case 1:
			if(v<vp) 
			{
				count++;
				flag=0;
			}
			break;
	}
}
void Seg_Pro()
{
	if(segslow) return ;
	segslow=1;
	v=Ad_Read(0x03)/51.0;
	switch(mode)
	{
		case 0 :
			seg[0]=12;
			seg[5]=(u8)v%10;
			seg[6]=(u8)(v*10)%10;
			seg[7]=(u16)(v*100)%10;
			pot[5]=1;
			break;
		case 1 :
			seg[0]=13;
			seg[5]=(u8)vp%10;
			seg[6]=(u8)(vp*10)%10;
			seg[7]=(u16)(vp*100)%10;
			pot[5]=1;
			break;
		case 2 :
			seg[0]=14;
			if(count/100)
				seg[5]=count/10;
			else
				seg[5]=10;
			if(count/10%10)
				seg[6]=count/10%10;
			else
				seg[6]=10;
			seg[7]=count%10;
			pot[5]=0;
			break;
	}
}
void Led_Pro()
{
	led[0]=led1;
	led[1]=count%2;
	led[3]=wrong>=3?1:0;
}
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR |= 0x80;			//定时器时钟1T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x20;				//设置定时初始值
	TH0 = 0xD1;				//设置定时初始值
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
	if(v<vp)
	{
		if(++delay5s==5000)
		{
			delay5s=0;
			led1=1;
		}
	}
	else
	{
		delay5s=0;
		led1=0;
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
	Ad_Read(0x03);
	sysinit();
	EEPROM_Read(&save,0,1);
	vp=save/10.0;
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		vcount();
	}
}
