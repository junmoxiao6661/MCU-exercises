#include <STC15F2K60S2.H>
#include <INTRINS.H>

typedef unsigned char u8;
typedef unsigned int u16;


void InitHC138(u8 n)
{
	switch(n)
	{
		case 4:P2=(P2&0x1F)|0x80;
	}
}

void main()
{
	
}
