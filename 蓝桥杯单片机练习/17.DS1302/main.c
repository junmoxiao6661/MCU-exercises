
#include <STC15F2K60S2.H>
#include "ds1302.h"


typedef unsigned char u8;
typedef unsigned int u16;


u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf};

u8 Time[3]={0x11,0x00,0x50};
void Delay(u16 xms)	//@12.000MHz
{
	while(xms--)
	{
		unsigned char i, j;

		i = 2;
		j = 239;
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
		case 0 :P2=(P2&0x1F)|0x00; break; //先&再|
		case 4 :P2=(P2&0x1F)|0x80; break;
		case 5 :P2=(P2&0x1F)|0xA0; break;
		case 6 :P2=(P2&0x1F)|0xC0; break;
		case 7 :P2=(P2&0x1F)|0xE0; break;
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

void DisplayTime()
{
	ShowSMG(SMG[Time[0]/16],0);
	ShowSMG(SMG[Time[0]%16],1);
	ShowSMG(SMG[10],2);
	ShowSMG(SMG[Time[1]/16],3);
	ShowSMG(SMG[Time[1]%16],4);
	ShowSMG(SMG[10],5);
	ShowSMG(SMG[Time[2]/16],6);
	ShowSMG(SMG[Time[2]%16],7);
}
void Set_Ds1302_pause(void)

{

	unsigned char temp;
	Write_Ds1302_Byte(0x8E,0X00); //允许写入
	temp=Read_Ds1302_Byte(0x81);//读当前时钟
	Write_Ds1302_Byte(0x80,temp|0x80);//置秒位高位为1,保持当前时钟
	Write_Ds1302_Byte(0x8E,0x80); //禁止写入

}

void main()
{
	InitHC138(4);
	P0=0xFF;
	InitHC138(0);
	Set_Rtc(Time);
	while(1)
	{
		Read_Rtc(Time);
		DisplayTime();
		if(Time[1]/16+Time[1]%16==1)
		{
			Set_Ds1302_pause();
		}
 	}
}