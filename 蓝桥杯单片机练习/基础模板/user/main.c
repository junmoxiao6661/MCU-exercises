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


unsigned char smg_mode;//0���ý��� 1�����ý���
unsigned char led_tier; //���ȵȼ� Rb2
unsigned char liu_z[4]={400,400,400,400};//�ĸ�ģʽ��Ӧ�ĸ���ת���
unsigned char liu_set[4]={400,400,400,400};//������ת�������
unsigned char index;//������������һ��Ӧ����ģʽ
unsigned char state;//0�����Ϩ�� 1ģʽ 2��ת���
unsigned int ms400;//�������˸ʱ��
bit smg_flag;//�������˸��־λ
unsigned char dat[4];//ר������ �����ݱ��浽EEPROM

void key_pro()
{
	unsigned char i=0;
	
	if(key_slow)return;
	key_slow=1;
	
	kval=key_read();
	kdown=kval &(kold ^ kval);//�жϰ���
	kup=~kval &(kold ^ kval);//�ж�����
	kold=kval;//�жϳ���
	switch(kdown)
	{
		case 6:
		
			if(++state==3)
				state=0;
			if(state==0) //�˻س�ʼ���汣������
			{
				index=0; //������λ
				for(i=0;i<4;i++)
				{
				liu_set[i]=liu_z[i];
				dat[i]=liu_set[i]/100;//ֻ��д0~255֮�����
				}
				eep_write(dat,0,4);//������ λ��0(һ��ҪΪ�˵ı���) 4������ 
			}
			break;
			
		case 5:		//�Ӱ���
		
				if(state==1)//ģʽ
				{
					if(++index==4)//0~3 ����ֵ 
						index=0;
				}
				else if(state==2)//��ת���
				{
					liu_z[index] += 100; //һ�μ�һ��
					if(++liu_z[index]>1200)
						liu_z[index]=1200; //��ס����
				}
	
			break;
			
			case 4:		//������
		
				if(state==1)//ģʽ
				{
					if(--index==255)//0~3 ����ֵ 
						index=3;
				}
				else if(state==2)//��ת���
				{
					liu_z[index] -=100; //һ�μ�һ��
					if(--liu_z[index]<400)
						liu_z[index]=400; //��ס����
				}
		
			break;
				
	}
	
}
void seg_pro()
{
	unsigned char i=0;

	if(seg_slow)return;
	seg_slow=1;
	
	led_tier=da_read(0x43)/64;//0��1��2��3��
	
	if(smg_mode==0)//����ģʽ��
	{
	seg_buf[0]=seg_buf[2]=11;//
	seg_buf[1]=index+1;
	
	seg_buf[4]=liu_z[index]/1000;
	seg_buf[5]=liu_z[index]/100%10;
	seg_buf[6]=liu_z[index]/10%10;
	seg_buf[7]=liu_z[index]%10;
		
		if(state==1)//ģʽ�����˸
		{
			seg_buf[0]=smg_flag?10:11;//��Ŀ����� ��˸�ܺ���
			seg_buf[2]=smg_flag?10:11;
			seg_buf[1]=smg_flag?10:index+1;
		}
		else if(state==2)//��ת�����˸
		{

		seg_buf[4]=smg_flag?10:liu_z[index]/1000;				
			seg_buf[4]=smg_flag?10:liu_z[index]/1000;
			seg_buf[5]=smg_flag?10:liu_z[index]/100%10;
			seg_buf[6]=smg_flag?10:liu_z[index]/10%10;
			seg_buf[7]=smg_flag?10:liu_z[index]%10;
		}
		if(seg_buf[4]==0)seg_buf[4]=10;
			
	}
	else //�����ý���
	{
		for(i=0;i<6;i++)
	seg_buf[i]=10;//���������Ϩ��
	seg_buf[6]=11;
	seg_buf[7]=led_tier;
	}
}
void led_pro()
{
	
}

void Timer0Init(void)		//1����@12.000MHz
{
	AUXR &= 0x7F;		//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;		//���ö�ʱ��ģʽ
	TL0 = 0x18;		//���ö�ʱ��ֵ
	TH0 = 0xFC;		//���ö�ʱ��ֵ
	TF0 = 0;		//���TF0��־
	TR0 = 1;		//��ʱ��0��ʼ��ʱ
	ET0=1;
	EA=1;
}

void Timer0s(void) interrupt 1
{
	if(++key_slow==10)key_slow=0;
	if(++seg_slow==500)seg_slow=0;
	if(++pos==8)pos=0;
	
	LED_dis(pos,uled[pos]);
	
	if(state!=0)  //s6��Ϊ0����ܲ��� ����Ϩ��
		seg_dis(pos,seg_buf[pos],pot[pos]);
	else
		seg_dis(pos,10,0);
	
	
	
	if(++ms400 ==400)
	{
		ms400=0;
		smg_flag^=1;//400msȡ��һ�� �ﵽ��˸Ч��
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
//		liu_z[i]=liu_set[i]=dat[i]*100;//�����ݶ����� �൱�ڰ�ԭ�����ݶ�ˢ��
	
	while(1)
	{
		key_pro();
		seg_pro();
		led_pro();
	}
}