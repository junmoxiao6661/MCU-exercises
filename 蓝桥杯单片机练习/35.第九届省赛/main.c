#include <STC15F2K60S2.H>
#include "iic.h"
#include "key.h"
#include "seg.h"
#include "led.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kslow,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u16 ledtime[4]={400,400,400,400};
u16 distime[4]={400,400,400,400};
u8 savetime[4];
u8 ledindex;
u8 ledmode,ledpos;
u8 level,ledcnt;
u16 delay400;
u8 segmode;
bit segflag;
u8 mode;
bit ledflag;
u16 leddelay;
void Key_Pro()
{
	u8 i;
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kold=kval;
	if(segmode==0)
	{
		if(kold==4)
			mode=1;
		else
			mode=0;
	}
	switch(kdown)
	{
		case 6:
			if(++segmode==3) segmode=0;
			if(segmode==0)
			{
				segmode=1;
				segmode=0;
				for(i=0;i<4;i++)
				{
					ledtime[i]=distime[i];
					savetime[i]=distime[i]/100;
				}
				EEPROM_Write(savetime,0,4);
			}
			break;
		case 5:
			if(segmode==1)
			{
				if(++ledindex==4) ledindex=0;
			}
			else if(segmode==2)
			{
				distime[ledindex]+=100;
				if(distime[ledindex]>1200)
					distime[ledindex]=1200;
			}
			break;
		case 4:
			if(segmode==1)
			{
				if(--ledindex==255) ledindex=3;
			}
			else if(segmode==2)
			{
				distime[ledindex]-=100;
				if(distime[ledindex]<400)
					distime[ledindex]=400;
			}
			break;
		case 7:
			ledflag^=1;
			break;
	}
}

void Seg_Pro()
{
	u8 i;
	if(segslow) return ;
	segslow=1;
	level=AD_Read(0x43)/64;
	if(mode==0)
	{
		seg[0]=11;
		seg[1]=ledindex+1;
		seg[2]=11;
		seg[4]=distime[ledindex]/1000==0?10:distime[ledindex]/1000;
		seg[5]=distime[ledindex]/100%10;
		seg[6]=distime[ledindex]/10%10;
		seg[7]=distime[ledindex]%10;
		if(segmode==1)
		{
			seg[0]=segflag?10:11;
			seg[1]=segflag?10:ledindex+1;
			seg[2]=segflag?10:11;
		}
		else if(segmode==2)
		{
			seg[4]=segflag?10:(distime[ledindex]/1000==0?10:distime[ledindex]/1000);
			seg[5]=segflag?10:distime[ledindex]/100%10;
			seg[6]=segflag?10:distime[ledindex]/10%10;
			seg[7]=segflag?10:distime[ledindex]%10;
		}
	}
	else
	{
		
		for(i=0;i<6;i++)
			seg[i]=10;
		seg[6]=11;
		seg[7]=level+1;
	}
}
	
void Led_Pro()
{
	u8 i;
	if(ledflag)
	{
		if(leddelay==distime[ledmode])
		{
			leddelay=0;
			switch(ledmode)
			{
				case 0:
					if(++ledpos==8)
					{
						ledpos=7;
						ledmode=1;
					}
					break;
				case 1:
					if(--ledpos==255)
					{
						ledpos=7;
						ledmode=2;
					}
					break;
				case 2:
					ledpos+=9;
					if(ledpos>34)
					{
						ledpos=34;
						ledmode=3;
					}
					break;
				case 3:
					ledpos-=9;
					if(ledpos>200)
					{
						ledpos=0;
						ledmode=0;
					}
					break;
			}
			
		}
	}
	if(ledmode<2)
	{
		for(i=0;i<8;i++)
		led[i]=ledflag&&(i==ledpos);
	}
	else
	{
		for(i=0;i<8;i++)
			led[i]=ledflag&&((i==ledpos/10)||(i==ledpos%10));
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
	ET0 = 1;				//使能定时器0中断
	EA=1;
}

void Timer0_Isr(void) interrupt 1
{
	if(++kslow==10) kslow=0;
	if(++segslow==8) segslow=0;
	if(++pos==8) pos=0;
	//if(++ledcnt==12) ledcnt=0;
	if(++delay400==400)
	{
		delay400=0;
		segflag^=1;
	}
	if(ledflag)
		leddelay++;
	if(mode||segmode)
		Seg_Display(pos,seg[pos],pot[pos]);
	else
		Seg_Display(pos,10,0);
	//if(ledcnt<=((level+1)*3))
	Led_Display(pos,led[pos]);
	//else
		//Led_Display(pos,0);
	
}
void sysinit()
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
	u8 i;
	sysinit();
	AD_Read(0x03);
	EEPROM_Read(savetime,0,4);
	for(i=0;i<4;i++)
	{
		distime[i]=savetime[i]*100;
	}
	Timer0_Init();
	while(1)
	{
		
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}
