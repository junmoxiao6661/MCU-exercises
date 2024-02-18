#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;
sbit S7 = P3^0;
sbit S6 = P3^1;
sbit S5 = P3^2;
sbit S4 = P3^3;
sbit L1 = P0^0;
sbit L2 = P0^1;
sbit L3 = P0^2;
sbit L4 = P0^3 ;
sbit L5 = P0^4 ;
sbit L6 = P0^5 ;


void InitHC138(u8 n)
{
	switch(n)
	{
		case 4: P2 = (P2&0x1F)|0x80; break;
		case 5: P2 = (P2&0x1F)|0xA0; break;
		case 6: P2 = (P2&0x1F)|0xC0; break;
		case 7: P2 = (P2&0x1F)|0xE0; break;
	}
}
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
u8 stat_k=0;
void ScanKeys_Alone() //按键扫描
{
	
	if(S7==0)
	{
		Delay(5); //消抖
		if(S7==0)
		{
			if(stat_k==0)
			{
				L1=0;
				stat_k=1;
			}
			else if(stat_k==1)
			{
				L1=1;
				stat_k=0;
			}
		}
		while(S7==0);//松手检测
	}
	if(S6==0)
	{
		Delay(5); //消抖
		if(S6==0)
		{
			if(stat_k==0)
			{
				L2=0;
				stat_k=2;
			}
			else if(stat_k==2)
			{
				L2=1;
				stat_k=0;
			}
		}
		while(S6==0);
	}
	if(S5==0)
	{
		Delay(5); //消抖
		if(S5==0)
		{
			if(stat_k==1)
			{
				while(S5==0)
					L3=0;
				L3=1;
			}
			else if(stat_k==2)
			{
				while(S5==0)
					L5=0;
				L5=1;
			}
		}
	}
	if(S4==0)
	{
		Delay(5); //消抖
		if(S4==0)
		{
			if(stat_k==1)
			{
				while(S4==0)
					L4=0;
				L4=1;
			}
			else if(stat_k==2)
			{
				while(S4==0)
					L6=0;
				L6=1;
			}
		}
	}
	
}
void main()
{
	InitHC138(4);
	P0=0xff;
	while(1)
	{
		
		ScanKeys_Alone();
	}
}
