#include <STC15F2K60S2.H>
#include <INTRINS.H>
sbit SCK=P1^7;
sbit SDA=P2^3;
sbit RST=P1^3;
void Read_Rtc(unsigned char *rtc);
void Set_Rtc(unsigned char *rtc);
