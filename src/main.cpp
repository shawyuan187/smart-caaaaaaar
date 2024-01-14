#include <Arduino.h>

//-----------------------全域變數區-----------------------//
// A1~A5為紅外線數值
int IR[5] = {A1, A2, A3, A4, A5};
// 速度控制:左邊馬達為PWM5，右邊馬達為PWM6
int mL = 5;
int mR = 6;
// 方向控制:左邊馬達為4，右邊馬達為7
int sL = 4;
int sR = 7;

//-----------------------指令宣告區-----------------------//

void motor(int, int);    // 控制馬達的函式
void IR_test();          // 紅外線測試函式
void forward();          // 前進函式
void small_turn_left();  // 小轉彎左轉函式
void small_turn_right(); // 小轉彎右轉函式
void mid_turn_left();    // 中轉彎左轉函式
void mid_turn_right();   // 中轉彎右轉函式
void big_turn_left();    // 大轉彎左轉函式
void big_turn_right();   // 大轉彎右轉函式
void stop();             // 停止函式
void trail();            // 循跡函式

//-----------------------初始化-----------------------//
void setup()
{
    Serial.begin(9600);
    // 將IR設定為輸入
    for (int i = 0; i < 5; i++)
    {
        pinMode(IR[i], INPUT); // 數值為0~1023，白色為0，黑色為1023
    }
    // 將mL和mR設定為輸出
    pinMode(mL, OUTPUT);
    pinMode(mR, OUTPUT);
    // 將sL和sR設定為輸出
    pinMode(sL, OUTPUT); // HIGH為正轉，LOW為反轉
    pinMode(sR, OUTPUT); // HIGH為正轉，LOW為反轉
}

//-----------------------主程式-----------------------//
void loop()
{
    trail();
}

//-----------------------函式區-----------------------//

void trail()
{
    if (analogRead(IR[2]) > 450)
    {
        if (analogRead(IR[1]) > 450)
        {
            mid_turn_left();
        }
        else if (analogRead(IR[3]) > 450)
        {
            mid_turn_right();
        }
        else if ((analogRead(IR[1]) < 450 && analogRead(IR[3]) < 450))
        {
            forward();
        }
    }   
    else
    {
        if (analogRead(IR[1]) > 450)
        {
            big_turn_left();
        }
        else if (analogRead(IR[3]) > 450)
        {
            big_turn_right();
        }
    }
}

/*
控制馬達的函式
speedL=左邊馬達的速度(-255 to 255)
speedR=右邊馬達的速度(-255 to 255)
*/
void motor(int speedL, int speedR)
{
    digitalWrite(sL, speedL < 0 ? LOW : HIGH); // 根據speedL的正負號來決定sL的HIGH或LOW
    // if (speedL < 0)
    // {
    // 	digitalWrite(sL, LOW);
    // }
    // else
    // {
    // 	digitalWrite(sL, HIGH);
    // }
    digitalWrite(sR, speedR < 0 ? LOW : HIGH); // 根據speedR的正負號來決定sR的HIGH或LOW
    // if (speedR < 0)
    // {
    // 	digitalWrite(sR, LOW);
    // }
    // else
    // {
    // 	digitalWrite(sR, HIGH);
    // }
    analogWrite(mL, abs(speedL)); // 馬達速度範圍為0~255
    analogWrite(mR, abs(speedR)); // 馬達速度範圍為0~255
}

void small_turn_left() // 小轉彎左轉函式
{
    motor(80, 150);
}

void small_turn_right() // 小轉彎右轉函式
{
    motor(150, 80);
}

void mid_turn_left()
{
    motor(0, 150);
}

void mid_turn_right()
{
    motor(150, 0);
}

void big_turn_left() // 大轉彎左轉函式
{
    motor(-150, 150);
}

void big_turn_right() // 大轉彎右轉函式
{
    motor(150, -150);
}

void stop() // 停止函式
{
    motor(0, 0);
}

void forward() // 前進函式
{
    motor(150, 150);    
}

/*
紅外線測試函式
*/
void IR_test()
{
    // 顯示IR的數值
    for (int i = 0; i < 5; i++)
    {
        Serial.print(analogRead(IR[i]));
        Serial.print(" ");
    }
    Serial.println();
}