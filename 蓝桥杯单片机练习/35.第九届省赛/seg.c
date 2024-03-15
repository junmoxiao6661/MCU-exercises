#include <STC15F2K60S2.H>
unsigned char code SMG[]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,0xff,0xbf};
unsigned char code wei[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
void Seg_Display(unsigned char i,j,k)
{
	P0=0xff;
	P2=P2&0x1f|0xe0;
	P2&=0x1f;
	
	P0=wei[i];
	P2=P2&0x1f|0xc0;
	P2&=0x1f;
	
	P0=SMG[j];
	if(k)
		P0&=0x7f;
	P2=P2&0x1f|0xe0;
	P2&=0x1f;
}
