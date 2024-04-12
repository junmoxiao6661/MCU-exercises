#include <STC15F2K60S2.H>
#include "led.h"
#include "seg.h"
#include "uart.h"
#include "key.h"
#include "ultrasound.h"
#include "ds1302.h"
#include "onewire.h"
#include "iic.h"

typedef unsigned char u8;
typedef unsigned int u16;

void Uart_Server() interrupt 4
{
	if(RI==1)
	{
		
		RI=0;
	}
}
void main()
{
	while(1)
	{
		
	}
}