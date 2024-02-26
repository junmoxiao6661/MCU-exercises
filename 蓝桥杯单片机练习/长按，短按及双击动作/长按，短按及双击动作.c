#include <STC15F2K60S2.H>
#include <Key.h>
#include <Led.h>

unsigned char led[8]={0,0,0,0,0,0,0,0};//led状态存储
unsigned char Seg_Pos,Key_Val,Key_Down,Key_Up,Key_Old,slow_down;
bit key_flag;
	
/*定义变量*/
unsigned int Key_Cnt_Flag = 0; //0——停止计时






/* 键盘处理函数 */
void Key_Proc()
{
	if(key_flag) return;
	key_flag = 1;//键盘减速程序

	Key_Val = Key_Read();//实时读取键码值
	Key_Down = Key_Val & (Key_Old ^ Key_Val);//捕捉按键下降沿
	Key_Up = ~Key_Val & (Key_Old ^ Key_Val);//捕捉按键上降沿
	Key_Old = Key_Val;//辅助扫描变量
	
	

		/*按键驱动中*/
	if(Key_Down==4){
		if(Key_Cnt_Flag){	//先进行判断双击动作
			led[2]^=1;//执行双击动作
			Key_Cnt_Flag = 0;	//复位
		}
		else{
			Key_Cnt_Flag = 1;	//开始计时
		}
	}

}

void Timer0_Init(void)		//1毫秒@12.000MHz
{
	AUXR &= 0x7F;			//定时器时钟12T模式
	TMOD &= 0xF0;			//设置定时器模式
	TL0 = 0x18;				//设置定时初始值
	TH0 = 0xFC;				//设置定时初始值
	TF0 = 0;				//清除TF0标志
	TR0 = 1;				//定时器0开始计时
	EA=1;
	ET0=1;
}

/* 定时器0中断服务函数 */
void Timer0Server() interrupt 1
{  
	slow_down++;slow_down%=200;
	if(slow_down%10==0) key_flag=0;
	
	if(++Seg_Pos == 8) Seg_Pos = 0;//数码管显示专用
	Led_Disp(Seg_Pos,led[Seg_Pos]);
	
	
	/*1ms定时器中*/

	if(Key_Cnt_Flag){				//长按计时部分
		if(++Key_Cnt_Flag==1000){	//以认为500毫秒为长按
			Key_Cnt_Flag = 0;
			if(Key_Val){	//没松手
				led[0]^=1;//执行长按动作
			}
			else{
				led[1]^=1;//执行短按动作
			}
		}
	}

}


void main()
{
	Timer0_Init();
	while(1)
	{
		Key_Proc();
	}
}