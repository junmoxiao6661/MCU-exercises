String rdStr="";
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.print("This is a demo program for string to integer");
 Serial.print("\n");
}
void loop() {
        while (Serial.available()) {
        char ch = Serial.read();
        Serial.print(ch,HEX);Serial.print("  ");
        if(isDigit(ch)) 
            rdStr=rdStr+ch;  
        else
            if(ch=='\r')    //if(ch=='\r')    \r->CR  \n->LF
          {
            Serial.print("\nThis is the input digital char :");
            Serial.println(rdStr);
            int input=rdStr.toInt();
            Serial.print("This is the input Integer Value :");
            Serial.println(input);
            rdStr="";
          }
        delay(50);   
    }
}
