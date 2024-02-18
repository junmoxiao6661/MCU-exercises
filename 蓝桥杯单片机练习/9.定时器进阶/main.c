#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
sbit S7 = P3^0;
u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e,0xbf,0x7f};
u8 min = 0;
u8 sec = 0;
u8 msec = 0;
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
    case 4 :
        P2=(P2&0X1F)|0x80;
        break;
    case 5 :
        P2=(P2&0X1F)|0xA0;
        break;
    case 6 :
        P2=(P2&0X1F)|0xC0;
        break;
    case 7 :
        P2=(P2&0X1F)|0xE0;
        break;
    }
}
void ShowSMG(u8 value,u8 pos)
{
    InitHC138(6);
    P0=(0x01)<<pos;
    InitHC138(7);
    P0=value;
	Delay(2);//消影
	P0=0xFF;
}
void Timer0_Init()		//50毫秒@12.000MHz
{
    AUXR &= 0x7F;			//定时器时钟12T模式
    TMOD &= 0xF0;			//设置定时器模式
    TMOD |= 0x01;			//设置定时器模式
    TL0 = 0xB0;				//设置定时初始值
    TH0 = 0x3C;				//设置定时初始值
    TF0 = 0;				//清除TF0标志
    EA=1;
    //TR0 = 1;				//定时器0开始计时
    ET0 = 1;				//使能定时器0中断
}
void Timer0_Isr() interrupt 1
{
    TL0 = 0xB0;				//设置定时初始值
    TH0 = 0x3C;				//设置定时初始值
	msec++;
	if(msec==20)
	{
		sec++;
		msec=0;
		if(sec==60)
		{
			min++;
			sec=0;
		}
	}
	if(min==99)
	{
		min=0;
	}
}

void DelaySMG(u16 t)
{
	while(t--);
}
void DisplayTime()
{
	ShowSMG(SMG[(msec*5)%10],7);
	ShowSMG(SMG[(msec*5)/10],6);
	ShowSMG(SMG[16],5);
	
	
	ShowSMG(SMG[sec%10],4);
	ShowSMG(SMG[sec/10],3);
	ShowSMG(SMG[16],2);
	
	
	ShowSMG(SMG[min%10],1);
	ShowSMG(SMG[min/10],0);
	
}

u8 KeyNum;
u8 Key()
{
	u8 res;
	if(S4==0){Delay(20);while(S4==0);Delay(20);res=4;};
	if(S5==0){Delay(20);while(S5==0);Delay(20);res=5;};
	if(S6==0){Delay(20);while(S6==0);Delay(20);res=6;};
	if(S7==0){Delay(20);while(S7==0);Delay(20);res=7;};
	return res;
}
void ScanKey()
{
    if(S4==0)//暂停
    {
        Delay(10);
        if(S4==0)
        {
			TR0 = ~TR0;
			while(S4==0)
			{
				DisplayTime();//按下时任然显示
			}
        }
		
    }
    if(S5==0)//清零
    {
        Delay(10);
        if(S5==0)
        {
			min=0;
			sec=0;
			msec=0;
			while(S5==0);
			{
				DisplayTime();
			}
        }
		
    }
	
}
void main()
{
    Timer0_Init();
    while(1)
    {
		
		ScanKey();
		DisplayTime();
    }
}
