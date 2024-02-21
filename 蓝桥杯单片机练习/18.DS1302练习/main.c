#include <STC15F2K60S2.H>
#include "ds1302.h"
typedef unsigned char u8;
typedef unsigned int u16;
sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0xbf};
u8 Time[]={0x12,0x12,0x56};
u8 hour,min,sec;
bit flag=0;
void Delay(u16 xms)		//@12.000MHz
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
		case 0 : P2=(P2&0x1F)|0x00; break;
		case 4 : P2=(P2&0x1F)|0x80; break;
		case 5 : P2=(P2&0x1F)|0xA0; break;
		case 6 : P2=(P2&0x1F)|0xC0; break;
		case 7 : P2=(P2&0x1F)|0xE0; break;
		
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
	Read_Rtc(Time);
	ShowSMG(SMG[Time[0]/16],0);
	ShowSMG(SMG[Time[0]%16],1);
	ShowSMG(SMG[10],2);
	ShowSMG(SMG[Time[1]/16],3);
	ShowSMG(SMG[Time[1]%16],4);
	ShowSMG(SMG[10],5);
	ShowSMG(SMG[Time[2]/16],6);
	ShowSMG(SMG[Time[2]%16],7);
	
}
void Pause_Time()
{
	if(flag==0)
	{
		Read_Rtc(Time);
		DisplayTime();
	}
	else
	{
		Set_Rtc(Time);
		DisplayTime();
	}
}
void ScanKey()
{
	if(S4==0)
	{
		Delay(10);
		if(S4==0)
		{
			while(S4==0)
			{
				DisplayTime();
			}
			flag=~flag;
		}
	}
	if(S5==0)
	{
		Delay(10);
		if(S5==0)
		{
			if(flag==1)
			{
				u8 temp = Time[1];
				temp = (temp/16)*10+temp%16;
				if(temp==0)
					temp=59;
				else
					temp--;
				Time[1]=(temp/10)*16+temp%10;//10->BCD
				Set_Rtc(Time);
			}
			while(S5==0)
			{
				DisplayTime();
			}
			
		}
	}
	if(S6==0)
	{
		Delay(10);
		if(S6==0)
		{
			if(flag==1)
			{
				u8 temp = Time[1];
				temp = (temp/16)*10+temp%16;
				if(temp==59)
					temp=0;
				else
					temp++;
				Time[1]=(temp/10)*16+temp%10;//10->BCD
				Set_Rtc(Time);
			}
			while(S6==0)
			{
				DisplayTime();
			}

		}
	}
}
void main()
{
	InitHC138(4);
	P0=0xFF;
	InitHC138(0);
	Set_Rtc(Time);
	while(1)
	{
		Pause_Time();
		ScanKey();
	}
}