#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;

u8 urdat;
void InitUart()
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
	EA = 1;   //总中断     

}



void SendByte(u8 dat)
{
	/*TI：发送中断标志位
	可寻址标志位。TI=1表示帧发送结束，
	TI可由软件清“0”。*/
	SBUF=dat;
	while(TI==0);
	TI=0;
}
void ServiceUart() interrupt 4 // 串口中断
{
	/*RI：接收中断标志位
	可寻址标志位。接收完第8位数据后，
	该位由硬件置位，在其他工作方式下，
	该位由硬件置位，RI=1表示帧接收完成。*/
	if(RI==1)
	{
		RI=0;
		urdat=SBUF;
		SendByte(urdat+1);
	}
	
}
void main()
{
	InitUart();
	SendByte(0x5a);
	SendByte(0xa5);
	while(1)
	{
		
	}
}
