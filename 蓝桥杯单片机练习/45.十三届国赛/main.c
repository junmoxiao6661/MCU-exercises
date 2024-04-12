#include <STC15F2K60S2.H>
#include "iic.h"
#include "ultrasound.h"
#include "key.h"
#include "seg.h"
#include "led.h"

typedef unsigned char u8;
typedef unsigned int u16;

u16 fredelay,fre;
u8 kslow,kval,kdown,kold;
u8 segslow,pos;
u8 seg[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 led[8]={0,0,0,0,0,0,0,0};
u8 mode,tmode;
u16 dis;
u8  rh;
float tempf=9.0;
u8 temprh=40;
float tempdis=0.6;
bit khz=0;
bit meter=0;
u8 ledflag=0;
u8 leddelay;
float rb2;
u8 cnt;
u16 delay;
bit cntflag;
bit flag=0;
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
		case 5:
			if(mode==3)
			{
				if(++tmode==3) tmode=0;
			}
			break;
		case 6:
			if(mode==2) meter^=1;
			if(mode==3)
			{
				if(tmode==0)
				{
					tempf+=0.5;
					if(tempf>12.0) tempf=1.0;
				}
				else if(tmode==1)
				{
					temprh+=10;
					if(temprh>60) temprh=10;
				}
				else if(tmode==2)
				{
					tempdis+=0.1;
					if(tempdis>1.2) tempdis=0.1;
				}
			}
			break;
		case 7:
			if(mode==0) khz^=1;
			if(mode==1)
			{
				cntflag=1;
				delay=0;
			}
			if(mode==3)
			{
				if(tmode==0)
				{
					tempf-=0.5;
					if(tempf<1.0) tempf=12.0;
				}
				else if(tmode==1)
				{
					temprh-=10;
					if(temprh<10) temprh=60;
				}
				else if(tmode==2)
				{
					tempdis-=0.1;
					if(tempdis<0.1) tempdis=1.2;
				}
			}
			break;
	}

}

void Seg_Pro()
{
	u8 i;
	u16 temp1;
	float temp2;
	if(segslow) return ;
	segslow=1;
	dis=Wave_Data();
	rb2=AD_Read(0x43)/51.0;
	switch(mode)
	{
		case 0:
			seg[0]=11;
			seg[1]=10;
			pot[5]=0;
			pot[6]=0;
			if(!khz)
			{
				temp1=fre;
				for(i=7;i>=3;i--)
				{
					if(temp1)
						seg[i]=temp1%10;
					else
						seg[i]=10;
					temp1/=10;
				}
			}
			else
			{
				temp1=fre;
				temp2=temp1/1000.0;
				seg[3]=10;
				seg[4]=10;
				seg[5]=(u8)temp2/10==0?10:(u8)temp2/10;
				seg[6]=(u8)temp2%10;
				pot[6]=1;
				seg[7]=(u16)(temp2*10)%10;
			}
			break;
		case 1:
			rh=rb2*20.0;
			if(rh>99) rh=99;
			seg[0]=12;
			seg[1]=10;
			seg[3]=10;
			seg[4]=10;
			seg[5]=10;
			seg[6]=rh/10;
			seg[7]=rh%10;
			pot[5]=0;
			pot[6]=0;
			break;
		case 2:
			seg[0]=13;
			seg[1]=10;
			seg[3]=10;
			seg[4]=10;
			pot[5]=0;
			pot[6]=0;
			if(!meter)
			{
				temp1=dis;
				for(i=7;i>=5;i--)
				{
					if(temp1)
						seg[i]=temp1%10;
					else
						seg[i]=10;
					temp1/=10;
				}
			}
			else
			{
				temp1=dis;
				temp2=temp1/100.0;
				seg[5]=(u8)temp2;
				pot[5]=1;
				seg[6]=(u8)(temp2*10)%10;
				seg[7]=(u16)(temp2*100)%10;
			}
			break;
		case 3:
			seg[0]=14;
			pot[5]=0;
			pot[6]=0;
			switch(tmode)
			{
				case 0:
					seg[1]=1;
					seg[5]=(u8)tempf/10==0?10:(u8)tempf/10;
					seg[6]=(u8)tempf%10;
					pot[6]=1;
					seg[7]=(u8)(tempf*10)%10;
					break;
				case 1:
					seg[1]=2;
					seg[5]=10;
					seg[6]=temprh/10;
					seg[7]=temprh%10;
					break;
				case 2:
					seg[1]=3;
					seg[6]=(u8)tempdis;
					pot[6]=1;
					seg[7]=(u8)(tempdis*10)%10;
					break;
			}
			break;
		/*case 4:
			seg[0]=10;
			seg[1]=10;
			pot[6]=0;
			seg[6]=cnt/10;
			seg[7]=cnt%10;
			break;*/
	}
}
void Led_Pro()
{
	led[0]=mode==0?ledflag:0;
	led[1]=mode==1?ledflag:0;
	led[2]=mode==2?ledflag:0;
	led[3]=fre>(tempf*1000);
	led[4]=rh>temprh;
	led[5]=dis>(tempdis*100);
	if(dis>(tempdis*100))
	{
		if(!flag)
		{
			flag=1;
			cnt++;
			EEPROM_Write(&cnt,0,1);
		}
		Relay(1);
		
	}
	else
	{
		Relay(0);
		flag=0;
	}
}
// NE555的TL、TH要初始化为0
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD |= 0x05;			//设置定时器模式
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
}


void Timer2_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xFB;			//定时器时钟12T模式
	T2L = 0x18;				//设置定时初始值
	T2H = 0xFC;				//设置定时初始值
	AUXR |= 0x10;			//定时器2开始计时
	IE2 |= 0x04;			//使能定时器2中断
	EA=1;
}
void Timer2_Isr(void) interrupt 12
{
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	
	Seg_Display(pos,seg[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(++fredelay==1000)
	{
		TR0=0;
		fre=TH0<<8|TL0;
		fredelay=0;
		TL0=TH0=0;
		TR0=1;
	}
	if(++leddelay==100) 
	{
		leddelay=0;
		ledflag^=1;
	}
	if(cntflag)
	{
		if(++delay==1000)
		{
			cntflag=0;
			cnt=0;
			EEPROM_Write(&cnt,0,1);
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
	//EEPROM_Read(&cnt,0,1);
	AD_Read(0x43);
	Timer2_Init();
	Timer0_Init();
	while(1)
	{
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
}
