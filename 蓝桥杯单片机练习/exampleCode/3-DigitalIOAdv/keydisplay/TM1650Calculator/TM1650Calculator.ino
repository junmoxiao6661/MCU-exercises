#include <TM1650.h>
#include <TM16xxDisplay.h>
#include <TM16xxButtons.h>
TM1650 module(A3, A2);  // SDA=A3, SCL=A2
TM16xxButtons buttons(&module);    // TM16xx button 
TM16xxDisplay display(&module, 4); // TM16xx object, 4 digits
void setup()
{
  module.clearDisplay();     // clear display
  module.setupDisplay(true, 3);  // set intensity 0-7, 7=highest
  module.setDisplayToString("HALO");
  delay(500);
  // Attach the button callback functions that are defined below
  buttons.attachClick(fnClick);
}
String  keyin;
char opreator;
int op1,op2;
bool isFirstOpreator=true;
bool isDone=false;
void loop()
{
  uint32_t dwButtons=buttons.tick();
  delay(200); // to keep the loop fast don't use delay(100) 
}
void fnClick(byte nButton)
{
  if(nButton<10)
  {    
    if(keyin.length()<4)
    {
      keyin=keyin+String(nButton);
      display.println(keyin);
    }
  }
  else
  {
    switch(nButton)
    {
      case 10:  opreator=1;
                if(isFirstOpreator){
                   if(keyin.length()>0)
                  { op1=keyin.toInt();
                    keyin="";
                    display.println(keyin);
                    isFirstOpreator=false;
                   } 
                }
                break;
      case 14:  if(isDone){
                    keyin="";
                    display.println(0);
                    isDone=false; 
                  }
                break;                  
      case 15:  if(!isFirstOpreator){
                  if(keyin.length()>0)
                  {
                    op2=keyin.toInt();
                    keyin="";
                    switch(opreator)
                    {
                      case 1:  op1=op1+op2;
                                display.println(op1);
                                isDone=true;
                                break;
                    }
                    isFirstOpreator=true; 
                  }
                }
                break;           
    }
  }
} // click
