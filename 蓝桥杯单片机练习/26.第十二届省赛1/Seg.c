#include <STC15F2K60S2.H>
unsigned char code segment[]={0xc0, //0
0xf9, //1
0xa4, //2
0xb0, //3
0x99, //4
0x92, //5
0x82, //6
0xf8, //7
0x80, //8
0x90, //9
0xff,//²»ϔʾ
0xbf,	
0x88, //A
0x83, //b
0xc6, //C
0xa1, //d
0x86, //E
0x8e, //F
0x8C  //P
};
code unsigned char wei[]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
void Seg_Display(unsigned char i,j,k)
{
	P0=0xFF;//消影
	P2=(P2&0x1F)|0xE0;
	P2&=0x1f;
	P0=wei[i];
	P2=P2&0x1f|0xc0;
	P2&=0x1f;
	P0=segment[j];
	if(k)
		P0&=0x7f;
	P2=P2&0x1f|0xe0;
	P2&=0x1f;
}