/* * 
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V 
 * */
#include <LiquidCrystal.h>       //LCD1602显示头文件
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); 
#define choose  8   //选择端口
#define add  6      //加
#define minus  7    //减
unsigned long seconds;
int s = 0, m = 0, h = 0, d = 0, mon = 0, y = 0;   //时间进位
int second = 0, minute = 0, hour = 0, day = 0, month = 0, year = 0;  //当前时间
int SECOND = 0, MINUTE = 0, HOUR = 0, DAY = 0, MONTH = 0, YEAR = 0;  //初始时间
int chose = 0, ButtonDelay = 10;
void setup(){    
    for(int i = 2;i <= 13; i++){
       pinMode(i,OUTPUT);  
    }          
    digitalWrite(add, HIGH);
    digitalWrite(minus, HIGH);
    digitalWrite(choose, HIGH);  
    lcd.begin(16, 2);  //初始化LCD1602 
    set(2015, 5, 17, 12, 30, 50);
}

/** 格式化输出  */
void FormatDisplay(int col, int row,int num){   
    lcd.setCursor(col, row); 
    if(num < 10){
        lcd.print("0");
    }   
    lcd.print(num);   
}

/** 计算时间 */
void time(){   
    second = (SECOND + seconds) % 60;   //计算秒
    m = (SECOND + seconds) / 60;        //分钟进位
    FormatDisplay(6,1,second);
    
    minute = (MINUTE + m) % 60;  //计算分钟
    h = (MINUTE + m) / 60;       //小时进位
    FormatDisplay(3,1,minute);      

    hour = (HOUR + h) % 24;   //计算小时
    d = (HOUR + h) / 24;      //天数进位
    FormatDisplay(0,1,hour); 
    
    lcd.setCursor(2, 1);   lcd.print(":");   
    lcd.setCursor(5, 1);   lcd.print(":");  
}

/** 根据年月计算当月天数 */
int Days(int year, int month){   
    int days = 0;
    if(month != 2){     
        switch(month){       
            case 1: case 3: case 5: case 7: case 8: case 10: case 12: days = 31;  break;
            case 4: case 6: case 9: case 11:  days = 30;  break;
        }
    } 
    else{   //闰年    
        if(year % 4 == 0 && year % 100 != 0 || year % 400 == 0){
            days = 29;
        }   
        else{
            days = 28;   
        }     
    }  
    return days;   
}

/** 计算当月天数 */
void Day(){      
    int days = Days(year,month);
    int days_up;
    if(month == 1){
        days_up = Days( year - 1, 12 );
    } else{
        days_up = Days( year, month - 1 );
    }  
    
    day = (DAY + d) % days;
    if(day == 0){
        day = days;    
    }   
    if((DAY + d) == days + 1){   
        DAY -= days;
        mon++;
    }
    if((DAY + d) == 0){
        DAY += days_up;
        mon--;
    }
    FormatDisplay(8,0,day); 
}

/** 计算月份 */
void Month(){   
    month = ( MONTH + mon ) % 12;
    if(month == 0){
        month = 12;
    }  
    y = (MONTH + mon - 1) / 12;
    FormatDisplay(5,0,month); 
    lcd.setCursor(7, 0);   
    lcd.print('-'); 
}

/** 计算年份 */
void Year(){     
    year = (YEAR + y) % 9999;
    if(year == 0){
        year = 9999;
    } 
    lcd.setCursor(0, 0); 
    if(year < 1000){ 
        lcd.print("0"); 
    }
    if(year < 100){ 
        lcd.print("0"); 
    }
    if(year < 10) { 
        lcd.print("0"); 
    }
    lcd.print(year);
    lcd.setCursor(4, 0);   
    lcd.print('-'); 
}

/** 根据年月日计算星期几 */
void Week(int y,int m, int d){        
    if(m == 1) m = 13;
    if(m == 2) m = 14;
    int week = (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400)%7+1; 
    String weekstr = "";
    switch(week){     
        case 1: weekstr = "Mon. ";   break;
        case 2: weekstr = "Tues. ";  break;
        case 3: weekstr = "Wed. ";   break;
        case 4: weekstr = "Thur. ";  break;
        case 5: weekstr = "Fri. ";   break;
        case 6: weekstr = "Sat. ";   break;
        case 7: weekstr = "Sun. ";   break;
    }    
    lcd.setCursor(11, 0);  lcd.print(weekstr);
}

/** 显示时间、日期、星期  */
void Display(){  
   time();
   Day();  
   Month();
   Year();
   Week(year,month,day);  
}

/** 显示光标 */
void DisplayCursor(int rol, int row){
    lcd.setCursor(rol, row);   
    lcd.cursor();
    delay(100);     
    lcd.noCursor();
    delay(100);    
}

/** 设置初始时间 */
void set(int y, int mon, int d, int h, int m, int s){
    YEAR = y;
    MONTH = mon;
    DAY = d;  
    HOUR = h;
    MINUTE = m;
    SECOND = s;  
}

/** 通过按键设置时间 */
void Set_Time(int rol, int row, int &Time){
   DisplayCursor(rol, row); 
   if(digitalRead(add) == LOW){
        delay(ButtonDelay);
        if(digitalRead(add) == LOW){
            Time ++;
        }           
        Display();      
   }  
   if(digitalRead(minus) == LOW){
        delay(ButtonDelay);
        if(digitalRead(minus) == LOW){
            Time --; 
        }           
        Display();  
    }  
}

/** 按键选择 */
void Set_Clock(){
    if(digitalRead(choose)==LOW){     
        lcd.setCursor(9, 1);  lcd.print("SetTime");
        while(1){       
            if(digitalRead(choose)==LOW){
                delay(ButtonDelay);
                if(digitalRead(choose)==LOW){
                    chose++;  
                }             
            } 
            seconds = millis()/1000; 
            Display(); 
            if(chose == 1){
                Set_Time(1, 1, HOUR);      //SetHour
            }else if(chose == 2){ 
                Set_Time(4, 1, MINUTE);    //SetMinute
            }else if(chose == 3){
                Set_Time(7, 1, SECOND);    //SetSecond
            }else if(chose == 4){
                Set_Time(9, 0, DAY);       //SetDay
            }else if(chose == 5){
                Set_Time(6, 0, MONTH);    // SetMonth
            }else if(chose == 6){           
                Set_Time(3, 0, YEAR);      //SetYear
            }else if(chose >= 7){
                chose = 0; 
                lcd.clear();
                break;
            }
        }
    }  
}

void loop(){ 
    seconds = millis()/1000;    //获取单片机当前运行时间 
    Display();       //显示时间
    Set_Clock();     //设置时间  
}
