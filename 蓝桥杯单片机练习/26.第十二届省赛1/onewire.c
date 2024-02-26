/*
  ³ÌĞòËµÃ÷: µ¥×ÜÏßÇı¶¯³ÌĞò
  Èí¼ş»·¾³: Keil uVision 4.10 
  Ó²¼ş»·¾³: CT107µ¥Æ¬»ú×ÛºÏÊµÑµÆ½Ì¨(Íâ²¿¾§Õñ12MHz) STC89C52RCµ¥Æ¬»ú
  ÈÕ    ÆÚ: 2011-8-9
*/
#include "reg52.h"

sbit DQ = P1^4;  //µ¥×ÜÏß½Ó¿Ú

//µ¥×ÜÏßÑÓÊ±º¯Êı
void Delay_OneWire(unsigned int t)  //STC89C52RC
{
	t*=12;//ä¸èƒ½å¿˜
	while(t--);
}

//Í¨¹ıµ¥×ÜÏßÏòDS18B20Ğ´Ò»¸ö×Ö½Ú
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(5);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(5);
}

//´ÓDS18B20¶ÁÈ¡Ò»¸ö×Ö½Ú
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(5);
	}
	return dat;
}

//DS18B20Éè±¸³õÊ¼»¯
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(12);
  	DQ = 0;
  	Delay_OneWire(80);
  	DQ = 1;
  	Delay_OneWire(10); 
    initflag = DQ;     
  	Delay_OneWire(5);
  
  	return initflag;
}
float rd_t(void)
{
	unsigned char low=0,high=0;
	unsigned int temp=0;
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0x44);
	
	init_ds18b20();
	Write_DS18B20(0xCC);
	Write_DS18B20(0xBE);
	low=Read_DS18B20();
	high=Read_DS18B20();
	temp=high;
	temp=(temp<<8)|low;
	
	return temp*0.0625;
}






