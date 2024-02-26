#include <STC15F2K60S2.H>
#include "Key.h"
#include "Seg.h"
#include "Led.h"
#include "iic.h"

typedef unsigned char u8;
typedef unsigned int u16;

u8 kval,kold,kup,kdown;
u8 kslow;
u8 segslow;
u8 segbuf[8]={10,10,10,10,10,10,10,10};
u8 pot[8]={0,0,0,0,0,0,0,0};
u8 pos=0;
u8 led[8]={0,0,0,0,0,0,0,0};
u16 fre=0,fre_delay=0;
u16 savef;
u8 saver;
u8 mode=0;
bit vmode=0;
float T=0.0;
float tempr;
u16 tempT;
u8 r1,r2;
u8 L,H;
u16 keyflag=0;
bit flag=0;
void Key_Pro()
{
	if(kslow) return;
	kslow=1;
	kval=Key_Read();
	kdown=kval&(kold^kval);
	kup=~kval&(kold^kval);
	kold=kval;
	if(kdown==4)
	{
		keyflag=1;
	}
	switch(kdown)
	{
		case 4:
			if(++mode==3) mode=0;
			break;
		case 5 :
			if(mode==2) vmode=~vmode;
			break;
		case 6 :
			if(vmode==1)  saver=r2;//EEPROM_Write(&r,0,1);
			break;
	}
	
	
}
void ShowNum(u16 n)
{
	u8 i;
	for(i=7;i>=1;i--)
	{
		if(n!=0)
		{
			segbuf[i]=n%10;
		}
		else
		{
			segbuf[i]=10;
		}
		n/=10;
	}
}


void Seg_Pro()
{
	unsigned char i=3;
	if(segslow) return;
	segslow=1;
	switch(mode)
	{
		case 0 :
			segbuf[0]=12;
			/*segbuf[1]=10;
			segbuf[2]=10;
			segbuf[3]=fre/10000%10;
			segbuf[4]=fre/1000%10;
			segbuf[5]=fre/100%10;
			segbuf[6]=fre/10%10;
			segbuf[7]=fre%10;
			while(segbuf[i]==0)
			{
				segbuf[i]=10;
				if(++i==7) break;
			}*/
			ShowNum(fre);
			pot[5]=0;
			break;
		case 1 :
			T=1.0/fre;
			tempT=T*1000000;
			segbuf[0]=13;
			/*segbuf[1]=10;
			segbuf[2]=10;
			segbuf[3]=tempT/10000%10;
			segbuf[4]=tempT/1000%10;
			segbuf[5]=tempT/100%10;
			segbuf[6]=tempT/10%10;
			segbuf[7]=tempT%10;
			while(segbuf[i]==0)
			{
				segbuf[i]=10;
				if(++i==7) break;
			}*/
			pot[5]=0;
			ShowNum(tempT);
			break;
		case 2 :
			if(vmode==0){r1=Ad_Read(0x41); tempr=r1/51.0;}
			else{r2=Ad_Read(0x43); tempr=r2/51.0;}
			segbuf[0]=14;
			segbuf[1]=11;
			if(vmode==0)
				segbuf[2]=1;
			else
			segbuf[2]=3;
			segbuf[3]=10;
			segbuf[4]=10;
			segbuf[5]=(u8)tempr%10;
			segbuf[6]=(u8)(tempr*10)%10;
			segbuf[7]=(u16)(tempr*100)%10;
			pot[5]=1;
			break;
	}
	
}
void Led_Pro()
{
	if(flag==0)
	{
		led[0]=(r2>saver);
		led[1]=(fre>savef);
		led[2]=(mode==0);
		led[3]=(mode==1);
		led[4]=(mode==2);
	}
}
void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD|=0x05;//0101
	TL0 = 0;				//设置定时初始值
	TH0 = 0;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0=0;
}

void Timer1_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0xBF;			//定时器时钟12T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0x18;				//设置定时初始值
	TH1 = 0xFC;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
	EA=1;
}
void TimerService() interrupt 3
{
	u16 temp=0;
	if(++kslow==10) kslow=0;
	if(++segslow==500) segslow=0;
	if(++pos==8) pos=0;
	Seg_Display(pos,segbuf[pos],pot[pos]);
	Led_Display(pos,led[pos]);
	if(keyflag&&kval==7)
	{
		if(++keyflag>=1000)
		{
			keyflag=0;
			flag=1;
		}
	}
	if(++fre_delay==1000)
	{
		TR0=0;
		fre_delay=0;
		temp=TH0;
		fre=(temp<<8)|TL0;
		TH0=TL0=0;
		TR0=1;
	}
	
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
	Timer1_Init();
	Timer0_Init();
	while(1)
	{
		
		Key_Pro();
		Seg_Pro();
		Led_Pro();
	}
	
}
