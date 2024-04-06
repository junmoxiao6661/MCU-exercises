#include <STC15F2K60S2.H>
#include "ds1302.h"
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
u8 time[3]={0x23,0x09,0x59};
u8 mode;
u8 arr[4];
u8 arr_pos;
u8 code key_map[19]={0,0,0,0,0,0,1,0,8,5,2,0,9,6,3,0,10,7,4};
u8 start_t[2];
u8 hsave,msave;
u8 dat=0;
u16 save;
u8 high,low;
bit flag=0;
bit saveflag=0;
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
			if(++mode==3) 
			{
				mode=0,arr[0]=10,arr[1]=10,arr[2]=10,arr[3]=10,arr_pos=0,save=0,flag=0,saveflag=0;
			}
			break;
		case 5:
			if(mode==1)
			{arr[0]=10,arr[1]=10,arr[2]=10,arr[3]=10,arr_pos=0,save=0,flag=0;}
			break;
	}
	if(key_map[kdown]&&mode==1)
	{
		if(!flag)
		{
			start_t[0]=time[0];
			start_t[1]=time[1];
			flag=1;
		}
		if(arr_pos<4)
			arr[arr_pos++]=key_map[kdown]-1;
	}
}
void Save_data()
{
	
}
void Seg_Pro()
{
	char i;
	u16 temp=0;
	if(segslow) return ;
	segslow=1;
	Read_Rtc(time);
	if(mode==2)
	{
		hsave=(start_t[0]/16)*10+start_t[0]%16;
		msave=(start_t[1]/16)*10+start_t[1]%16;
			Write(0,hsave);
			Write(1,msave);
			high=save/256;
			low=save%256;
			Write(2,high);
			Write(3,low);
	}
	switch(mode)
	{
		case 0:
			seg[0]=time[0]/16;
			seg[1]=time[0]%16;
			seg[2]=11;
			seg[3]=time[1]/16;
			seg[4]=time[1]%16;
			seg[5]=11;
			seg[6]=time[2]/16;
			seg[7]=time[2]%16;
			break;
		case 1:
			seg[0]=12;
			seg[1]=10;
			seg[2]=10;
			seg[3]=10;
			for(i=0;i<arr_pos;i++)
				temp=temp*10+arr[i];
			seg[4]=arr_pos<4?10:temp/1000;
			seg[5]=arr_pos<3?10:temp/100%10;
			seg[6]=arr_pos<2?10:temp/10%10;
			seg[7]=arr_pos<1?10:temp%10;
			save=temp;
			break;
		case 2:
			seg[0]=13;
			seg[1]=10;
			seg[2]=10;
			seg[3]=start_t[0]/16;
			seg[4]=start_t[0]%16;
			seg[5]=11;
			seg[6]=start_t[1]/16;
			seg[7]=start_t[1]%16;
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
void main()
{
	sysinit();
	Set_Rtc(time);
	start_t[0]=Read(0)/10*16+Read(0)%10;
	start_t[1]=Read(1)/10*16+Read(1)%10;
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		
	}
}
