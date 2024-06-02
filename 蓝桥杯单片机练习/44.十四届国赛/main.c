#include <STC15F2K60S2.H>
#include "onewire.h"
#include "iic.h"
#include "key.h"
#include "led.h"
#include "seg.h"
#include "ultrasound.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 mode;
u8 kval,kold,kdown,kslow;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
char changedis=0;
u16 velocity=340;
float dalow=1.0;
float t;
u16 dis;
bit cm=0,tmode=0;
u8 fmode;
u8 tempdis=40,tempt=30;
bit restart;
u16 delay,leddelay;
u16 writedis;
bit writeflag=0;
u8 ledflag=0;
u16 writedelay;
void Init()
{
	mode=0;
	cm=0;
	tempdis=40;
	tempt=30;
	changedis=0;
	velocity=340;
	dalow=1.0;

}
void Key_Pro()
{
	if(kslow) return;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kold=kval;
	if(!writeflag)
	{
		switch(kdown)
		{
			case 4:
				if(++mode==3) mode=0;
				break;
			case 5:
				if(mode==0) cm^=1;
				if(mode==1) tmode^=1;
				if(mode==2) 
				{
					if(++fmode==3) fmode=0;
				}
				break;
			case 8:
				if(mode==0)
					writeflag=1;
				if(mode==1)
				{
					if(tmode==0)
					{
						tempdis+=10;
						if(tempdis>90) tempdis=90;
					}
					else
					{
						if(++tempt>80) tempt=80;
					}
				}
				if(mode==2)
				{
					if(fmode==0)
					{
						changedis+=5;
						if(changedis>90) changedis=90;
					}
					else if(fmode==1)
					{
						velocity+=10;
						if(velocity>9990) velocity=9990;
					}
					else if(fmode==2)
					{
						dalow+=0.1;
						if(dalow>2.0) dalow=2.0;
					}
				}
				break;
			case 9:
				if(mode==0)
				{
					if(writedis<=dalow)
						DA_Write(dalow*51.0);
					else if(writedis>=90)
						DA_Write(255);
					else
						DA_Write(((5.0-dalow)/80.0)*writedis);
				}
				if(mode==1)
				{
					if(tmode==0)
					{
						tempdis-=10;
						if(tempdis<10) tempdis=10;
					}
					else
					{
						if(--tempt==255) tempt=0;
					}
				}
				if(mode==2)
				{
					if(fmode==0)
					{
						changedis-=5;
						if(changedis<-90) changedis=-90;
					}
					else if(fmode==1)
					{
						velocity-=10;
						if(velocity<10) velocity=10;
					}
					else if(fmode==2)
					{
						dalow-=0.1;
						if(dalow<0.1) dalow=0.1;
					}
				}
				break;
			case 89:
				delay=0;
				restart=1;
				break;
		}
	}
	else
		writedis=dis;
}
void Seg_Pro()
{
	u8 i;
	u16 temp;
	float temp1;
	if(segslow) return ;
	segslow=1;
	t=Read_T();
	dis=Wave_Data(changedis,velocity);
	switch(mode)
	{
		case 0:
			seg[0]=(u8)t/10;
			seg[1]=(u8)t%10;
			pot[1]=1;
			seg[2]=(u16)(t*10)%10;
			seg[3]=11;
			temp=dis;
			pot[5]=0;
			pot[6]=0;
			if(!cm)
			{
				
				for(i=7;i>=4;i--)
				{
					if(temp)
						seg[i]=temp%10;
					else
						seg[i]=10;
					temp/=10;
				}
			}
			else
			{
				temp1=dis/100.0;
				seg[4]=10;
				seg[5]=(u8)temp1;
				pot[5]=1;
				seg[6]=(u8)(temp1*10)%10;
				seg[7]=(u16)(temp1*100)%10;
			}
			break;
		case 1:
			seg[0]=12;
			seg[1]=tmode==0?1:2;
			pot[1]=0;
			pot[5]=0;
			pot[6]=0;
			for(i=2;i<=5;i++)
				seg[i]=10;
			seg[6]=tmode==0?tempdis/10:tempt/10;
			seg[7]=tmode==0?tempdis%10:tempt%10;
			break;
		case 2:
			seg[0]=13;
			pot[6]=0;
			switch (fmode)
			{
				case 0:
					seg[1]=1;
					if(changedis>=0)
					{
						seg[4]=seg[5]=10;
						seg[6]=changedis/10==0?10:changedis/10;
						seg[7]=changedis%10;
					}
					else
					{
						i=-changedis;
						if(i<=9)
						{
							seg[4]=seg[5]=10;
							seg[6]=11;
							seg[7]=i;
						}
						else
						{
							seg[4]=10;
							seg[5]=11;
							seg[6]=i/10;
							seg[7]=i%10;
						}
					}
					break;
				case 1:
					seg[1]=2;
					temp=velocity;
					for(i=7;i>=4;i--)
					{
						if(temp)
							seg[i]=temp%10;
						else
							seg[i]=10;
						temp/=10;
					}
					break;
				case 2:
					seg[1]=3;
					seg[4]=seg[5]=10;
					seg[6]=(u8)dalow;
					seg[7]=(u8)(dalow*10)%10;
					pot[6]=1;
					break;
			}
			break;
	}
}
void Led_Pro()
{
	u8 i;
	u16 temp;
	if(mode==0)
	{
		ledflag=0;
		temp=dis;
		if(temp<=255)
		{
			for(i=0;i<8;i++)
			{
				led[i]=temp%2;
				temp/=2;
			}
		}
		else
		{
			for(i=0;i<8;i++)
				led[i]=1;
		}
	}
	else if(mode==1)
	{
		for(i=0;i<7;i++)
			led[i]=0;
		led[7]=1;
		leddelay=0;
	}
	else
	{
		for(i=1;i<8;i++)
			led[i]=0;
		led[0]=ledflag;
		
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
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(writeflag)
	{
		if(++writedelay==6000)
		{
			writeflag=0;
			writedelay=0;
		}
	}
	if(restart&&kval==89) 
	{
		if(++delay==2000)
		{
			Init();
			delay=0;
			restart=0;
		}
	}
	if(++leddelay==100) 
	{
		ledflag^=1;
		leddelay=0;
	}
	Relay((dis>=(tempdis-5)&&dis<=(tempdis+5))&&t<=tempt);
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
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}
