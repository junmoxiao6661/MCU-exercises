String rdStr="";
void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
 Serial.print("Hello World");
 Serial.print("\n");
}
void loop() {
        while (Serial.available()) {
        char ch = Serial.read();
        Serial.print(ch,DEC);
        if(isDigit(ch)|| ch=='.') 
            rdStr=rdStr+ch;  
        else
            if(ch=='\n') 
          {
            Serial.print("This is the input digital char :");
            Serial.println(rdStr);
            float input=rdStr.toFloat();
            Serial.print(input);
            rdStr="";
          }
        delay(50);   
    }
}
