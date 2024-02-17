 #define seg_a 2    //定义数字接口7 连接a 段数码管
 #define seg_b 3    // 定义数字接口6 连接b 段数码管
 #define seg_c 4    // 定义数字接口5 连接c 段数码管
 #define seg_d 5   // 定义数字接口11 连接d 段数码管
 #define seg_e 6   // 定义数字接口10 连接e 段数码管
 #define seg_f 7    // 定义数字接口8 连接f 段数码管
 #define seg_g 8    // 定义数字接口9 连接g 段数码管
 #define seg_dp 9   // 定义数字接口4 连接dp 段数码管
void setup() {
        
        pinMode(seg_a,OUTPUT);
        pinMode(seg_b,OUTPUT);
        pinMode(seg_c,OUTPUT);
        pinMode(seg_d,OUTPUT);
        pinMode(seg_e,OUTPUT);
        pinMode(seg_f,OUTPUT);
        pinMode(seg_g,OUTPUT);
        pinMode(seg_dp,OUTPUT);
        Serial.begin(9600);
}
int number;
void loop() {
  number =0;
  display(number);
  Serial.write("0");
  delay(1000);
  number =1;
  display(number);
  Serial.write("1");
  Serial.write("-----");
  delay(1000);
  }  
  void display(int number)
  {
      switch(number)
  {
    case 0:
        digitalWrite(seg_a,HIGH);
        digitalWrite(seg_b,HIGH);
        digitalWrite(seg_c,HIGH);
        digitalWrite(seg_d,HIGH);
        digitalWrite(seg_e,HIGH);
        digitalWrite(seg_f,HIGH);
        digitalWrite(seg_g,HIGH);
        digitalWrite(seg_dp,HIGH);
        //digitalWrite(seg_a,LOW);
        //digitalWrite(seg_b,LOW);
        //digitalWrite(seg_c,LOW);
        digitalWrite(seg_d,LOW);
        digitalWrite(seg_e,LOW);
        digitalWrite(seg_f,LOW);
        digitalWrite(seg_g,LOW);
        digitalWrite(seg_dp,LOW);
        break; 
    case 1:
        digitalWrite(seg_a,LOW);
        digitalWrite(seg_b,LOW);
        digitalWrite(seg_c,LOW);
        digitalWrite(seg_d,LOW);
        digitalWrite(seg_e,LOW);
        digitalWrite(seg_f,LOW);
        digitalWrite(seg_g,LOW);
        digitalWrite(seg_dp,LOW);
        break; 
  } 
  }
  void displaydp(int number)
  {
      switch(number)
  {
    case 0:
        digitalWrite(seg_a,LOW);
        digitalWrite(seg_b,LOW);
        digitalWrite(seg_c,LOW);
        digitalWrite(seg_d,LOW);
        digitalWrite(seg_e,LOW);
        digitalWrite(seg_f,LOW);
        digitalWrite(seg_g,HIGH);
        digitalWrite(seg_dp,LOW);
        break; 
    case 1:
        digitalWrite(seg_a,HIGH);
        digitalWrite(seg_b,LOW);
        digitalWrite(seg_c,LOW);
        digitalWrite(seg_d,HIGH);
        digitalWrite(seg_e,HIGH);
        digitalWrite(seg_f,HIGH);
        digitalWrite(seg_g,HIGH);
        digitalWrite(seg_dp,LOW);
        break; 
  }  
  }                     
