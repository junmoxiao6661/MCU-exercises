#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
#include "key.h"
#include "led.h"
#include "seg.h"
#include "ultrasound.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kdown,kslow,kold,kup;
u8 pos;
u16 segslow;
u8 seg[8]={10,10,10,10,10,10,10,10};
xdata u8 led[8]={0,0,0,0,0,0,0,0};
xdata u8 pot[8]={0,0,0,0,0,0,0,0};
char sstring[20];
char rstring[10];
u8 index=0;
bit mode=0;
u8 dmode=0,pmode=0;
u8 dis;
float t;
u8 changecnt;
char pt=30,pd=35;
char segt=30,segd=35;
bit daflag=1;
u16 delay1s=0;
bit key12flag=0,key13flag=0;
u8 uartslow=0;
void Key_Pro()
{
	if(kslow) return ;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kval^kold);
	kup=~kval&(kval^kold);
	kold=kval;
	if(kdown==12)
	{
		key12flag=1;
	}else if(kdown==13)
	{
		key13flag=1;
	}
	if(kup)
	{
		if(delay1s>=1000)
		{
			delay1s=0;
			if(key12flag)
			{
				key12flag=0;
				changecnt=0;
			}
			else if(key13flag)
			{
				key13flag=0;
				daflag^=1;
			}
		}
		else
		{
			switch(kup)
			{
				case 13:
					if(mode==0)
						mode=1,pmode=0;
					else
					{
						mode=0,dmode=0;
						if(pd!=segd||pt!=segt)
						{
							changecnt++;
							EEPROM_Write(&changecnt,0,1);
						}
						pd=segd,pt=segt;
						
					}
					break;
				case 12:
					key12flag=1;
					delay1s=0;
					if(mode==0)
					{
						if(++dmode==3) dmode=0;
					}
					else
					{
						if(++pmode==2) pmode=0;
					}
					break;
				case 16:
					if(mode==1)
					{
						if(pmode==0)
						{
							segt-=2;
							if(segt<0) segt=0;
						}
						else
						{
							segd-=5;
							if(segd<0) segd=0;
						}
					}
					break;
				case 17:
					if(mode==1)
					{
						if(pmode==0)
						{
							segt+=2;
							if(segt>99) segt=99;
						}
						else
						{
							segd+=5;
							if(segd>99) segd=99;
						}
					}
					break;
			}
		}
	}
}

void Seg_Pro()
{
	u8 i;
	u16 j;
	if(segslow) return ;
	segslow=1;
	t=rd_t();
	dis=Wave_Data();
	if(daflag)
	{
		if(dis<=pd)
			DA_Write(2*51);
		else
			DA_Write(4*51);
	}
	else
		DA_Write(20);
	if(mode==0)
	{
		seg[1]=seg[2]=seg[3]=seg[4]=seg[5]=seg[6]=10;
		pot[5]=0;
		switch(dmode)
		{
			case 0:
				seg[0]=11;
				pot[5]=1;
				seg[4]=(u8)(t)/10;
				seg[5]=(u8)(t)%10;
				seg[6]=(u16)(t*10)%10;
				seg[7]=(u16)(t*100)%10;
				break;
			case 1:
				seg[0]=12;
				seg[6]=dis/10;
				seg[7]=dis%10;
				break;
			case 2:
				seg[0]=13;
				seg[7]=changecnt%10;
				j=changecnt/10;
				i=6;
				while(j!=0)
				{
					seg[i]=j%10;
					j/=10;
					i--;
				}
				break;
		}
	}
	else
	{
		seg[0]=14;
		seg[1]=seg[2]=seg[4]=seg[5]=10;
		pot[5]=0;
		switch(pmode)
		{
			case 0:
				seg[3]=1;
				seg[6]=segt/10;
				seg[7]=segt%10;
				break;
			case 1:
				seg[3]=2;
				seg[6]=segd/10;
				seg[7]=segd%10;
				break;
		}
	}
}

void Led_Pro()
{
	led[0]=t>pt;
	led[1]=dis<pd;
	led[2]=daflag;
}

void Uart_Proc()
{
	if(uartslow) return ;
	uartslow=1;
	if(index>1)
	{
		if(strcmp(rstring,"ST\r\n")==0)
		{
			sprintf(sstring,"$%2d,%.2f\r\n",(unsigned int)dis,t);
			SendString(sstring);
		}
		else if(strcmp(rstring,"PARA\r\n")==0)
		{
			sprintf(sstring,"#%d,%d\r\n",(unsigned int)pd,(unsigned int)pt);
			SendString(sstring);
		}
		else
		{
			sprintf(sstring,"%s","ERROR\r\n");
			SendString(sstring);
		}
		index=0;
		memset(rstring,0,10);
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
	ET0=1;
	EA=1;
}

void Timer0_Service() interrupt 1
{
	if(++kslow==10) kslow=0;
	if(++segslow==200) segslow=0;
	if(++pos==8) pos=0;
	if((key12flag&&kval==12)||(key13flag&&kval==13))
	{
		++delay1s;
	}
	if(++uartslow==100) uartslow=0;
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
void Uart1Server() interrupt 4
{
	if(RI==1)
	{
		rstring[index]=SBUF;
		index++;
		RI=0;
	}
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

void main()
{
	rd_t();
	EEPROM_Read(&changecnt,0,1);
	Delay750ms();
	sysinit();
	UartInit();
	
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
		Uart_Proc();
		
	}
}