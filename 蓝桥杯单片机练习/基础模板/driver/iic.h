#ifndef __IIC_H__
#define __IIC_H__
unsigned char da_read(unsigned char addr);
void da_write(unsigned char dat);
void eep_write(unsigned char * dat,unsigned char addr,unsigned char num);
void eep_read(unsigned char * dat,unsigned char addr,unsigned char num);
#endif