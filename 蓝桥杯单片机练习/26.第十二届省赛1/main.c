#include <STC15F2K60S2.H>
#include <INTRINS.H>
#include "onewire.h"
#include "iic.h"
#include "key.h"
#include "Seg.h"
#include "Led.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 key_val,key_d,key_u,key_o;
u8 key_slow;
u8 seg_slow;
u8 pos=0;
u8 seg_buf[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 Led[8]={0,0,0,0,0,0,0,0};
u8 mode=0;
u8 temp_d=25,temp_c=25;
bit outv_mode=0;
float T;
float V;
void Delay750ms()		//@12.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
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



void Key_Pro()
{
	if(key_slow) return;
	key_slow=1;
	
	key_val=key_read();
	key_d=key_val&(key_o^key_val);
	key_u=~key_val&(key_o^key_val);
	key_o=key_val;
	
	switch(key_d)
	{
		case 4 :
			if(++mode==3) mode=0;
			if(mode==1) temp_d=temp_c;
			if(mode==2) temp_c=temp_d;
			break;
		case 8 :
			if(mode==1)
			{
				if(--temp_d==255)
					temp_d=0;
			}
			break;
		case 9:
			if(mode==1)
			{
				if(++temp_d==100)
					temp_d=99;
			}
			break;
		case 5 :
			outv_mode=~outv_mode;
			break;
		
	}
}
void OutV()
{
	if(outv_mode==0)
	{
		if(T<temp_c)
			V=0.00;
		else
			V=5.00;
	}
	else
	{
		if(T<=20)
			V=1.0;
		else if(T>=20&&T<=40)
			V=0.15*T-2.0;
		else
			V=4.0;
	}
	Da_Write((u8)51*V);
}
void Seg_Pro()
{
	if(seg_slow)return;
	seg_slow=1;
	T=rd_t();
	OutV();
	//temp = T*100;
	switch(mode)
	{
		case 0 ://2456
			seg_buf[0]=14;
			seg_buf[4]=(u8)T/10%10;
			seg_buf[5]=(u8)T%10;
			seg_buf[6]=(u16)(T*100)/10%10;//注意范围
			seg_buf[7]=(u16)(T*100)%10;
			pot[5]=1;
			break;
		case 1 :
			seg_buf[0]=18;
			seg_buf[4]=10;
			seg_buf[5]=10;
			seg_buf[6]=temp_d/10;
			seg_buf[7]=temp_d%10;
			pot[5]=0;
			break;
		case 2 :
			seg_buf[0]=12;
			seg_buf[4]=10;
			seg_buf[5]=(u8)V;
			seg_buf[6]=(u8)(V*10)%10;
			seg_buf[7]=(u16)(V*100)%10;//注意范围
			pot[5]=1;
			break;
			
	}
}
void Led_Pro()
{
	Led[0]=~outv_mode;
	Led[1]=(mode==0);
	Led[2]=(mode==1);
	Led[3]=(mode==2);
}
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA=1;
}
void Timer0Service() interrupt 1
{
	if(++key_slow==10) key_slow=0;
	if(++seg_slow==500) seg_slow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg_buf[pos],pot[pos]);
	LED_Display(pos,Led[pos]);
}
void sys_init()
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2&=0x1f;
	
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2&=0x1f;
}
void main()
{
	sys_init();
	rd_t();
	Delay750ms();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}