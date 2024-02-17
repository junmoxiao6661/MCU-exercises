#include <TM1650.h>
TM1650 module(A3, A2);  // SDA=A3, SCL=A2
char  table[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x30,0x40};
void setup()
{
  module.setDisplayToString("HALO");
  delay(1000);  
}
void loop()
{
  module.clearDisplay();
  for(byte nPos=0; nPos<4; nPos++)     
  {
    for(int nVal=0; nVal<=0xFF; nVal=nVal*2+1)    
    {
      module.setSegments(nVal, nPos);   
      delay(500);
    }
  }
  delay(2000);
  module.clearDisplay();
    for(byte nPos=0; nPos<4; nPos++)   
  {
    for(int nVal=0; nVal<=17; nVal=nVal+1)     
    {
      module.setSegments(table[nVal], nPos); 
      delay(800);
    }
  }
}
