#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;
//串口接受数据为中断模式
//发送为查询模式
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
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
	ES = 1; //串口中断
	EA = 1; //总中断
}


u8 command;
void SendByte(u8 dat) //一个字节一个字节发送
{
	SBUF=dat;//缓冲区
	while(TI==0); //TI=1发送完结束
	TI=0;//重置
}

void SendString(u8 *str)  //字符串就是一堆字符组成
{
	while(*str!='\0')
	{
		SendByte(*str++);
	}
	
}
void ServiceUart() interrupt 4
{
	if(RI==1)
	{
		command = SBUF; //接受外部数据
		RI=0;
	}
}

void Working()
{
	if(command!=0x00)
	{
		switch(command & 0xF0)//提取前四位
		{
			case 0xA0:
				P0 = (P0|0x0F)&(~command|0xF0);
				command=0;
				break;
			case 0xB0:
				P0 = (P0|0xF0)&((~command<<4)|0x0F);
				command=0;
				break;
			case 0xC0:
				SendString("The System is Running...\n");
				command=0;
				break;

		}
	} 
}
void main()
{
	InitHC138(5);
	P0=0x00; //蜂鸣器和继电器由达林顿管驱动，为高电平驱动
	InitHC138(4);
	P0=0xFF;
	UartInit();
	SendString("Welcome to XMF system！ \n");
	while(1)
	{
		 Working();
	}
}