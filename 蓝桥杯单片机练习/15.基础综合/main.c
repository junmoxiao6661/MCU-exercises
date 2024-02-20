#include <STC15F2K60S2.H>

typedef unsigned char u8;
typedef unsigned int u16;

sbit S4 = P3^3;
sbit S5 = P3^2;
u8 code SMG[]= {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x80, 0xc6, 0xc0, 0x86, 0x8e,0xbf,0x7f};
void InitHC138(u8 n)
{
	switch(n)
	{
		case 4: P2=(P2&0x1F)|0x80; break;
		case 5: P2=(P2&0x1F)|0xA0; break;
		case 6: P2=(P2&0x1F)|0xC0; break;
		case 7: P2=(P2&0x1F)|0xE0; break;
		case 0: P2=(P2&0x1F)|0x00; break;//锁存器不选择
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
u8 count,sec,min,hour;


void Timer0_Init(void)		//50毫秒@11.0592MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TMOD |= 0x01;			//设置定时器模式
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0x4C;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	ET0 = 1;				//使能定时器0中断
	EA = 1;
}

void Timer0_Isr(void) interrupt 1
{
	TL0 = 0x00;				//设置定时初始值
	TH0 = 0x4C;				//设置定时初始值
	count++;
	if(count==20)
	{
		count=0;
		sec++;
	}
	if(sec==60)
	{
		sec=0;
		min++;
		if(min==60)
		{
			min=0;
			hour++;
		}
	}
}
void UartInit(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	AUXR &= 0xBF;		//定时器时钟12T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;
	TR1 = 1;
	ES = 1;
	EA = 1;
}
u8 command;


void SendByte(u8 dat)
{
	SBUF = dat;
	while(TI==0);
	TI=0;
}
void ServiceUart() interrupt 4
{
	if(RI==1)
	{
		command = SBUF;
		RI=0;
	}
}
void ShowSMG(u8 value,u8 pos)
{
	InitHC138(6);
	P0=(0x01<<pos);
	InitHC138(7);
	P0=value;
	Delay(2);
	P0=0xFF;//消影
}
void DisplayTime()
{
	ShowSMG(SMG[hour/10],0);
	ShowSMG(SMG[hour%10],1);
	ShowSMG(SMG[16],2);
	ShowSMG(SMG[min/10],3);
	ShowSMG(SMG[min%10],4);
	ShowSMG(SMG[16],5);
	ShowSMG(SMG[sec/10],6);
	ShowSMG(SMG[sec%10],7);
}

void ExcuteCommand()
{
	if(command!=0x00)
	{
		switch(command&0xF0)
		{
			case 0xA0:
				InitHC138(4);
			    P0=(P0|0x0F)&(~command|0xF0);
				InitHC138(0);
				command=0x00;
				break;
			case 0xB0:
				SendByte((hour/10<<4)|(hour%10));
				SendByte((min/10<<4)|(min%10));
				SendByte((sec/10<<4)|(sec%10));
				command=0;
				break;
				
		}
	}
}
void InitSystem()
{
	InitHC138(5);
	P0=0x00;
	InitHC138(4);
	P0=0xFF;
	InitHC138(0);
}
void CheckLED()
{
	u8 i;
	InitHC138(4);
	for(i=1;i<=8;i++)
	{
		P0=0xFF<<i;
		Delay(1000);
	}
	for(i=1;i<=8;i++)
	{
		P0=~(0xFF<<i);
		Delay(1000);
	}
	InitHC138(0);
}
void CheckSMG()
{
	u8 i;
	InitHC138(7);
	P0=0x00;
	for(i=1;i<=8;i++)
	{
		InitHC138(6);
		P0=~(0xFF<<i);//哪个为1哪个亮
		Delay(1000);
	}
	for(i=1;i<=8;i++)
	{
		InitHC138(6);
		P0=0xFF<<i;
		Delay(1000);
	}
	InitHC138(0);
}
u8 stat_led=0xFF;
void ScanKey()
{
	if(S5==0)
	{
		Delay(10);
		DisplayTime();
		if(S5==0)
		{
			while(S5==0)
			{
				DisplayTime();
			}
			InitHC138(4);
			stat_led=(stat_led | 0x40)&(~stat_led|0xbf);
			P0=stat_led;
			InitHC138(0);
		}
	}
	if(S4==0)
	{
		Delay(10);
		DisplayTime();
		if(S4==0)
		{
			while(S4==0)
			{
				DisplayTime();
			}
			InitHC138(4);
			stat_led=(stat_led | 0x80)&(~stat_led|0x7f);
			P0=stat_led;
			InitHC138(0);
		}
	}
}
void main()
{
	
	InitSystem();
	CheckLED();
	CheckSMG();
	Timer0_Init();
	UartInit();	
	SendByte(0x66);
	while(1)
	{
		ExcuteCommand();
		DisplayTime();
		ScanKey();
	}
}