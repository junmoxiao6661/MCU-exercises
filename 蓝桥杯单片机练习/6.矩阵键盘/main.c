#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit R1 = P3^0;
sbit R2 = P3^1;
sbit R3 = P3^2;
sbit R4 = P3^3;

sbit C1 = P4^4;
sbit C2 = P4^2;
sbit C3 = P3^5;
sbit C4 = P3^4;

u8 code SMG[]={0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e,0xc2};

void Delay(u16 xms)		//@11.0592MHz
{
	while(xms--)
	{
		unsigned char i, j;

		_nop_();
		i = 2;
		j = 199;
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
		case 4 : P2=(P2&0x1F)|0x80; break;
		case 5 : P2=(P2&0x1F)|0xA0; break;
		case 6 : P2=(P2&0x1F)|0xC0; break;
		case 7 : P2=(P2&0x1F)|0xE0; break;
	}
}
void DispalySMG(u8 value,u8 loc)
{
	P0=0xFF;
	InitHC138(6); // 位选
	P0=(0x01)<<loc;
	InitHC138(7); // 段选
	P0=SMG[value];
	Delay(1); //消影
	P0=0xFF;
}
u8 KeyNum = 0;
void ScanKeysMulti()
{
	C1 = C2 = C3 =C4=1;
	
	P3=0xFF;
	R1=0;
	if(C1==0){Delay(10);while(C1==0);Delay(10);KeyNum=1;}
	if(C2==0){Delay(10);while(C2==0);Delay(10);KeyNum=2;}
	if(C3==0){Delay(10);while(C3==0);Delay(10);KeyNum=3;}
	if(C4==0){Delay(10);while(C4==0);Delay(10);KeyNum=4;}
	
	P3=0xFF;
	R2=0;
	if(C1==0){Delay(10);while(C1==0);Delay(10);KeyNum=5;}
	if(C2==0){Delay(10);while(C2==0);Delay(10);KeyNum=6;}
	if(C3==0){Delay(10);while(C3==0);Delay(10);KeyNum=7;}
	if(C4==0){Delay(10);while(C4==0);Delay(10);KeyNum=8;}
	
	P3=0xFF;
	R3=0;
	if(C1==0){Delay(10);while(C1==0);Delay(10);KeyNum=9;}
	if(C2==0){Delay(10);while(C2==0);Delay(10);KeyNum=10;}
	if(C3==0){Delay(10);while(C3==0);Delay(10);KeyNum=11;}
	if(C4==0){Delay(10);while(C4==0);Delay(10);KeyNum=12;}
	
	P3=0xFF;
	R4=0;
	if(C1==0){Delay(10);while(C1==0);Delay(10);KeyNum=13;}
	if(C2==0){Delay(10);while(C2==0);Delay(10);KeyNum=14;}
	if(C3==0){Delay(10);while(C3==0);Delay(10);KeyNum=15;}
	if(C4==0){Delay(10);while(C4==0);Delay(10);KeyNum=16;}
	
}
void main()
{
	InitHC138(4);
	P0=0xFF;
	 while(1)
	 {
		 ScanKeysMulti();
		 DispalySMG(KeyNum,0);
	 }
}