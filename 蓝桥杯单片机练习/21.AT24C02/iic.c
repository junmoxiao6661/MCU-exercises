/*
  ³ÌÐòËµÃ÷: IIC×ÜÏßÇý¶¯³ÌÐò
  Èí¼þ»·¾³: Keil uVision 4.10 
  Ó²¼þ»·¾³: CT107µ¥Æ¬»ú×ÛºÏÊµÑµÆ½Ì¨ 8051£¬12MHz
  ÈÕ    ÆÚ: 2011-8-9
*/

#include "reg52.h"
#include "intrins.h"

#define DELAY_TIME 5

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//×ÜÏßÒý½Å¶¨Òå
sbit SDA = P2^1;  /* Êý¾ÝÏß */
sbit SCL = P2^0;  /* Ê±ÖÓÏß */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//×ÜÏßÆô¶¯Ìõ¼þ
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//×ÜÏßÍ£Ö¹Ìõ¼þ
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//·¢ËÍÓ¦´ð
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0£ºÓ¦´ð£¬1£º·ÇÓ¦´ð
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//µÈ´ýÓ¦´ð
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//Í¨¹ýI2C×ÜÏß·¢ËÍÊý¾Ý
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//´ÓI2C×ÜÏßÉÏ½ÓÊÕÊý¾Ý
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}
void EEPROM_Write(unsigned char* EEPROM_String,unsigned char addr,unsigned char num)//addrä¸º8çš„å€æ•°
{
	IIC_Start();
	IIC_SendByte(0xA0);// å†™æ“ä½œ
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	while(num--)
	{
		IIC_SendByte(*EEPROM_String++);
		IIC_WaitAck();
		IIC_Delay(200);
	}
	IIC_Stop();
}
void EEPROM_Read(unsigned char* EEPROM_String,unsigned char addr,unsigned char num)
{
	IIC_Start();
	IIC_SendByte(0xA1);
	IIC_WaitAck();
	IIC_SendByte(addr);
	IIC_WaitAck();
	
	while(num--)
	{
		*EEPROM_String++=IIC_RecByte();
		if(num) IIC_SendAck(0);
			else IIC_SendAck(1);
	}
	IIC_Stop();
}