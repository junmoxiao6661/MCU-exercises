#include <STC15F2K60S2.H>
#include "onewire.h"
#include <INTRINS.H>
typedef unsigned char u8;
typedef unsigned int u16;
sbit S4 = P3^3;
u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf};
unsigned char code SMG_Dot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
u8 MODE=0;
float t=0;
u16 temp=0;
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

void ShowSMG(u8 value,u8 pos)
{
	InitHC138(6);
	P0=0x01<<pos;
	InitHC138(7);
	P0=value;
	Delay(2);
	P0=0xFF;
}
void DisplayT()
{
	
	switch(MODE)
	{
		case 0 :
			temp=t*100;
			if(temp/1000!=0)
				ShowSMG(SMG[temp/1000],4);
			ShowSMG(SMG_Dot[(temp/100)%10],5);
			ShowSMG(SMG[(temp/10)%10],6);
			ShowSMG(SMG[temp%10],7);
			break;
		case 1 :
			temp=t*10;
			if(temp/100!=0)
				ShowSMG(SMG[temp/100],5);
			ShowSMG(SMG_Dot[(temp/10)%10],6);
			ShowSMG(SMG[temp%10],7);
			break;
		case 2 :
			temp=t;
			if(temp/10!=0)
				ShowSMG(SMG[temp/10],6);
			ShowSMG(SMG[temp%10],7);
			break;
		
	}
}
float rd_tempertature(void)
{
	unsigned char low=0,high=0;
	unsigned int temp=0;
	//float T;
	init_ds18b20();
	DisplayT();
	Write_DS18B20(0xCC); // 跳过ROM
	Write_DS18B20(0x44); // 温度转换
	DisplayT();
	init_ds18b20();
	DisplayT();
	Write_DS18B20(0xCC); // 跳过ROM
	Write_DS18B20(0xBE); // 获取温度
	DisplayT();
	low = Read_DS18B20();
	high = Read_DS18B20();
	DisplayT();
	temp=high;
	temp=(temp<<8)|low;
	DisplayT();
	return temp*0.0625;
}


void DelaySMG(u16 t)
{
	while(t--);
}
	
void ScanKey()
{
	if(S4==0)
	{
		DelaySMG(100);
		if(S4==0)
		{
			while(S4==0)
			{
				DisplayT();
			}
			MODE++;
			//DisplayT();
			if(MODE>2)
				MODE=0;
		}
		
	}
}
// 本次代码主要问题在于DS18B20读取高精度数据有750ms延迟因此会造成数码管闪烁以及变为0
// 的情况，解决方法可以在读取数据时多执行Display
void main()
{
	rd_tempertature();
	InitHC138(4);
	P0=0xFF;
	InitHC138(0);
	//Delay(750);
	while(1)
	{
		t=rd_tempertature();
		ScanKey();
		
	}
	
}