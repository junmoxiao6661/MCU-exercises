#include <STC15F2K60S2.H>
#include "iic.h"
#include "ultrasound.h"
#include "led.h"
#include "key.h"
#include "seg.h"
typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kold,kslow;
u8 pos;
u8 segslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 dis,disc;
u8 mode;
u8 lowd=10,highd=60;
u8 pmode=0;
u8 cnt=0;
u8 v;
bit hflag=0,lflag=0,ledflag=0;
u8 ledcnt;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kold=kval;
	if(pmode==0)
		hflag=lflag=0;
	switch(kdown)
	{
		case 4:
			if(++mode==3) mode=0;
			break;
		case 5:
			if(mode==1)
				if(++pmode==2) pmode=0;
			if(mode==2)
				cnt=0;
			break;
		case 9:
			if(mode==1&&pmode==0)
			{
				highd+=10;
				if(highd==100) highd=50;
			}
			if(mode==1&&pmode==1) 
			{
				hflag=1;
				lflag=0;
			}
			break;
		case 8:
			if(mode==1&&pmode==0)
			{
				lowd+=10;
				if(lowd==50) lowd=0;
			}
			if(mode==1&&pmode==1) 
			{
				lflag=1;
				hflag=0;
			}
			break;
	}
}
void Seg_Pro()
{
	if(segslow) return;
	segslow=1;
	dis=Ut_Wave_Data();
	v=Ad_Read(0x03);
	if(dis>=lowd&&dis<=highd)
	{
		ledflag=0;
		ledcnt=0;
		led[7]=1;
	}
	if((dis>highd||dis<lowd)&&ledflag==0)
	{	
		led[7]=0;
		cnt++;
		ledflag=1;
	}
	if(hflag)
	{
		if(v<51)
			highd=50;
		else if(v>=51&&v<102)
			highd=60;
		else if(v>=102&&v<153)
			highd=70;
		else if(v>=153&&v<204)
			highd=80;
		else
			highd=90;
	}
	if(lflag)
	{
		if(v<51)
			lowd=0;
		else if(v>=51&&v<102)
			lowd=10;
		else if(v>=102&&v<153)
			lowd=20;
		else if(v>=153&&v<204)
			lowd=30;
		else
			lowd=40;
	}
	switch(mode)
	{
		case 0:
			seg[0]=12;
			seg[1]=10;
			seg[3]=10;
			seg[4]=10;
			seg[5]=dis/100==0?10:dis/100;
			seg[6]=dis/10%10==0&&dis/100==0?10:dis/10%10;
			seg[7]=dis%10;
			break;
		case 1:
			seg[0]=13;
			seg[1]=pmode+1;
			seg[3]=lowd/10;
			seg[4]=lowd%10;
			seg[5]=11;
			seg[6]=highd/10;
			seg[7]=highd%10;
			break;
		case 2 :
			seg[0]=14;
			seg[1]=10;
			seg[3]=10;
			seg[4]=10;
			seg[5]=10;
			seg[6]=10;
			if(cnt<=9)
				seg[7]=cnt;
			else
				seg[7]=11;
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
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(ledflag)
	{
		if(++ledcnt==100)
		{
			led[7]=~led[7];
			ledcnt=0;
		}
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
	Timer0_Init();
    while(1)
    {
        Key_Pro();
		Seg_Pro();
		Led_Pro();
    }
}