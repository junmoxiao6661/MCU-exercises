#ifndef _IIC_H
#define _IIC_H

void IIC_Start(void); 
void IIC_Stop(void);  
bit IIC_WaitAck(void);  
void IIC_SendAck(bit ackbit); 
void IIC_SendByte(unsigned char byt); 
unsigned char IIC_RecByte(void); 
void Write_EEPROM(unsigned char* EEPROM_String,unsigned char addr,unsigned char num);
void Read_EEPROM(unsigned char* EEPROM_String,unsigned char addr,unsigned char num);
#endif