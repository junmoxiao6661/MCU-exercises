#include <STC15F2K60S2.H>
#include "key.h"
#include "led.h"
#include "seg.h"
#include "iic.h"

unsigned char pos;
unsigned char kval,kdown,kup,kold;
unsigned char key_slow;
unsigned int seg_slow;
unsigned char seg_buf[8]={10,10,10,10,10,10,10,10};
unsigned char pot[8]={0,0,0,0,0,0,0,0};
unsigned char uled[8]={0,0,0,0,0,0,0,0};


unsigned char smg_mode;//0设置界面 1非设置界面
unsigned char led_tier; //亮度等级 Rb2
unsigned char liu_z[4]={400,400,400,400};//四个模式对应四个流转间隔
unsigned char liu_set[4]={400,400,400,400};//设置流转间隔数据
unsigned char index;//数组索引，加一对应四种模式
unsigned char state;//0数码管熄灭 1模式 2流转间隔
unsigned int ms400;//数码管闪烁时间
bit smg_flag;//数码管闪烁标志位
unsigned char dat[4];//专用数组 把数据保存到EEPROM

void key_pro()
{
	unsigned char i=0;
	
	if(key_slow)return;
	key_slow=1;
	
	kval=key_read();
	kdown=kval &(kold ^ kval);//判断按下
	kup=~kval &(kold ^ kval);//判断松手
	kold=kval;//判断长按
	switch(kdown)
	{
		case 6:
		
			if(++state==3)
				state=0;
			if(state==0) //退回初始界面保存数据
			{
				index=0; //索引复位
				for(i=0;i<4;i++)
				{
				liu_set[i]=liu_z[i];
				dat[i]=liu_set[i]/100;//只能写0~255之间的数
				}
				eep_write(dat,0,4);//数组名 位置0(一定要为八的倍数) 4个数据 
			}
			break;
			
		case 5:		//加按键
		
				if(state==1)//模式
				{
					if(++index==4)//0~3 索引值 
						index=0;
				}
				else if(state==2)//流转间隔
				{
					liu_z[index] += 100; //一次加一百
					if(++liu_z[index]>1200)
						liu_z[index]=1200; //卡住上限
				}
	
			break;
			
			case 4:		//减按键
		
				if(state==1)//模式
				{
					if(--index==255)//0~3 索引值 
						index=3;
				}
				else if(state==2)//流转间隔
				{
					liu_z[index] -=100; //一次减一百
					if(--liu_z[index]<400)
						liu_z[index]=400; //卡住上限
				}
		
			break;
				
	}
	
}
void seg_pro()
{
	unsigned char i=0;

	if(seg_slow)return;
	seg_slow=1;
	
	led_tier=da_read(0x43)/64;//0，1，2，3级
	
	if(smg_mode==0)//设置模式下
	{
	seg_buf[0]=seg_buf[2]=11;//
	seg_buf[1]=index+1;
	
	seg_buf[4]=liu_z[index]/1000;
	seg_buf[5]=liu_z[index]/100%10;
	seg_buf[6]=liu_z[index]/10%10;
	seg_buf[7]=liu_z[index]%10;
		
		if(state==1)//模式编号闪烁
		{
			seg_buf[0]=smg_flag?10:11;//三目运算符 闪烁很好用
			seg_buf[2]=smg_flag?10:11;
			seg_buf[1]=smg_flag?10:index+1;
		}
		else if(state==2)//流转间隔闪烁
		{

		seg_buf[4]=smg_flag?10:liu_z[index]/1000;				
			seg_buf[4]=smg_flag?10:liu_z[index]/1000;
			seg_buf[5]=smg_flag?10:liu_z[index]/100%10;
			seg_buf[6]=smg_flag?10:liu_z[index]/10%10;
			seg_buf[7]=smg_flag?10:liu_z[index]%10;
		}
		if(seg_buf[4]==0)seg_buf[4]=10;
			
	}
	else //非设置界面
	{
		for(i=0;i<6;i++)
	seg_buf[i]=10;//其他数码管熄灭
	seg_buf[6]=11;
	seg_buf[7]=led_tier;
	}
}
void led_pro()
{
	
}

void Timer0Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;		//定时器时钟12T模式
	TMOD &= 0xF0;		//设置定时器模式
	TL0 = 0x18;		//设置定时初值
	TH0 = 0xFC;		//设置定时初值
	TF0 = 0;		//清除TF0标志
	TR0 = 1;		//定时器0开始计时
	ET0=1;
	EA=1;
}

void Timer0s(void) interrupt 1
{
	if(++key_slow==10)key_slow=0;
	if(++seg_slow==500)seg_slow=0;
	if(++pos==8)pos=0;
	
	LED_dis(pos,uled[pos]);
	
	if(state!=0)  //s6不为0数码管才亮 否则熄灭
		seg_dis(pos,seg_buf[pos],pot[pos]);
	else
		seg_dis(pos,10,0);
	
	
	
	if(++ms400 ==400)
	{
		ms400=0;
		smg_flag^=1;//400ms取反一下 达到闪烁效果
	}
}	

void sys_init()
{
	P0=0xff;
	P2=P2&0x1f|0x80;
	P2&=0x1f;
	
	P0=0x00;
	P2=P2&0x1f|0xa0;
	P2&=0x1f;
}

void main()
{	
	unsigned char i=0;
	sys_init();
	Timer0Init();
//	eep_read(dat,0,4);
//	
//	for(i=0;i<4;i++)
//		liu_z[i]=liu_set[i]=dat[i]*100;//把数据读出来 相当于把原来数据都刷新
	
	while(1)
	{
		key_pro();
		seg_pro();
		led_pro();
	}
}