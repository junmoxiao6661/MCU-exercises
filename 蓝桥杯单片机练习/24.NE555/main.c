#include <STC15F2K60S2.H>

typedef unsigned char u8;
typedef unsigned int u16;
u16 count=0,sec=0,fre=0;
u8 code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xbf};
void Delay(u16 xms)		//@12.000MHz
{
	while(xms--)
	{
		unsigned char i, j;

		i = 12;
		j = 169;
		do
		{
			while (--j);
		} while (--i);
	}
}

void InitHC138(u8 n)
{
	switch(n)
	{
		case 0 : P2=(P2&0x1F)|0x00; break;
		case 4 : P2=(P2&0x1F)|0x80; break;
		case 5 : P2=(P2&0x1F)|0xA0; break;
		case 6 : P2=(P2&0x1F)|0xC0; break;
		case 7 : P2=(P2&0x1F)|0xE0; break;
	}
}

void ShowSMG(u8 val,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=val;
	Delay(2);
	P0=0xFF;
}
void Timer_Init(void)
{
	TMOD = 0x16; //0001 0110
	ET0=1;
	ET1=1;
	EA=1;
	TH0=0xFF;
	TL0=0xFF;
	TL1=0x18;
	TH1=0xFC;
	TR0=1;
	TR1=1;
}
void Timer0_Service() interrupt 1
{
	count++;
}

void Timer1_Service() interrupt 3
{
	TL1=0x18;
	TH1=0xFC;
	sec++;
	if(sec>=1000)
	{
		fre=count;
		count=0;
		sec=0;
	}
}
void DisplayF()
{
	u8 i;
	u16 temp=fre;
	for(i=5;i>=1;i--)
	{
		ShowSMG(SMG[temp%10],i+2);
		temp/=10;
	}
}
void main()
{
	InitHC138(4);
	P0=0xFF;
	Timer_Init();
	while(1)
	{
		DisplayF();
	}
}