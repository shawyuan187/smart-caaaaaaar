#include <Arduino.h>
#include <Servo.h>

/*################################變數宣告區################################*/
// IR sansor A1~A5
int IR[5] = {A1, A2, A3, A4, A5};
// 速度控制:左邊馬達為PWM5，右邊馬達為PWM6
int mL = 5;
int mR = 6;
// 方向控制:左邊馬達為4，右邊馬達為7
int sL = 4;
int sR = 7;
Servo claw; // 爪子伺服馬達
Servo arm;  // 手臂伺服馬達
/*################################函數宣告區################################*/
// 馬達控制
void motor(int, int);
// 循跡
void trail();
// 小轉彎_左
void small_turn_left();
// 小轉彎_右
void small_turn_right();
// 中轉彎_左
void mid_turn_left();
// 中轉彎_右
void mid_turn_right();
// 大轉彎_左
void big_turn_left();
// 大轉彎_右
void big_turn_right();
// 前進
void forward();
// 停止
void stop();
// 後退
void back();
// 回到線用_左
void re_turn_left();
// 回到線_左
void return_to_line_left();
// 回到線用_右
void re_turn_right();
// 回到線_右
void return_to_line_right();
// 回到線_2
void return_to_line_2();
// 循跡直到路口
void trail_cross();
// 判斷是否為路口
bool cross();
// 初始化
void my_init();
// 夾起
void pick_up();
// 放下
void pick_down();
// 放下_2
void pick_down_2();
// 手臂上升
void arm_up();
// 手臂下降
void arm_down();
// 爪子打開
void claw_open();
// 爪子閉合
void claw_close();
// 循跡一段時間
void trail_for_ms(unsigned long ms);
// 搬運第一個貨物
void pick_first();
// 循跡直到IR1~IR3都看到白色
void forward_to_white();
// 執行某個動作直到設定的某個IR先看到黑色再看到白色
void action_until_black_to_white(int IR, void (*action)());
// 執行某個動作直到IR1~IR3其中一個看到黑色
void action_until_black(void (*action)());
/*################################程式初始化################################*/
void setup() // 程式初始化
{
    my_init();
    pick_up();
    pick_first();
}

/*################################程式循環################################*/
void loop() // 程式循環
{
}

/*################################函數定義區################################*/

void pick_first()
{
    trail_cross();
    forward_to_white();
    return_to_line_right();
    pick_down();
    trail_cross();
    pick_up();
    action_until_black_to_white(IR[0], back);
    return_to_line_left();
    trail_cross();
    action_until_black_to_white(IR[4], forward);
    action_until_black(re_turn_right);
    trail_cross();
    action_until_black_to_white(IR[0], forward);
    action_until_black(re_turn_left);
    trail_cross();
    trail_for_ms(1000);
    big_turn_right();
    delay(600);
    stop();
    forward();
    delay(300);
    stop();
    claw_open();
}

void action_until_black(void (*action)())
{
    while (!(analogRead(IR[1]) > 450 or analogRead(IR[2]) > 450 or analogRead(IR[3]) > 450))
    {
        action();
    }
    stop();
}

void action_until_black_to_white(int IR, void (*action)())
{
    while (!(analogRead(IR) > 450))
    {
        action();
    }
    while (!(analogRead(IR) < 450))
    {
        action();
    }
    stop();
    delay(100);
}

void forward_to_white()
{
    while (!(analogRead(IR[1]) < 450 and analogRead(IR[2]) < 450 and analogRead(IR[3]) < 450))
    {
        forward();
    }
    stop();
    delay(100);
}

void trail_for_ms(unsigned long ms)
{
    unsigned long start_time = millis();
    while (millis() - start_time < ms)
    {
        trail();
    }
    stop();
}

void arm_up()
{
    arm.write(80); // 調整手臂角度範例
}

void arm_down()
{
    // arm.write(120); // 調整手臂角度範例
    for (int i = 0; i < 3; i++)
    {
        arm.write(100 + 10 * i);
        delay(100);
    }
}

void claw_open()
{
    claw.write(5);
}

void claw_close()
{
    claw.write(100);
}

void pick_up()
{
    claw_close();
    delay(100);
    arm_up();
}

void pick_down()
{
    arm_down();
    delay(100);
    claw_open();
    delay(100);
}

void pick_down_2()
{
    arm.write(120);
    delay(100);
    claw.write(20); // 調整爪子角度
}

void my_init()
{
    for (int i = 0; i < 5; i++)
    {
        pinMode(IR[i], INPUT);
    }

    pinMode(mL, OUTPUT);
    pinMode(mR, OUTPUT);
    pinMode(sL, OUTPUT);
    pinMode(sR, OUTPUT);

    claw.attach(9);
    arm.attach(10);
}

void return_to_line_left()
{
    while (!(analogRead(IR[0]) > 450))
    {
        re_turn_left();
    }
    stop();
}

void return_to_line_right()
{
    while (!(analogRead(IR[4]) > 450))
    {
        re_turn_right();
    }
    stop();
}
void return_to_line_2()
{
    while (!(analogRead(IR[0]) > 450))
    {
        re_turn_left();
    }
    stop();
}

void trail_cross()
{
    while (!cross())
    {
        trail();
    }
    while (!(analogRead(IR[1]) < 450 or analogRead(IR[2]) < 450 or analogRead(IR[3]) < 450))
    {
        forward();
    }
    stop();
    delay(100);
}

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
        else if (analogRead(IR[1]) < 450 and analogRead(IR[3]) < 450)

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

bool cross()
{
    // 判斷IR1~IR3有沒有同時大於450，如果有的話就停止

    if (analogRead(IR[1]) > 450 and analogRead(IR[2]) > 450 and analogRead(IR[3]) > 450)
    {
        stop();
        return true;
    }
    else
    {
        return false;
    }
}

void forward()
{
    motor(100, 100);
}

void back()
{
    motor(-120, -120);
}
void small_turn_left()
{
    motor(80, 100);
}

void small_turn_right()
{
    motor(100, 80);
}

void mid_turn_left()
{
    motor(0, 110);
}
void mid_turn_right()
{
    motor(110, 0);
}

void big_turn_left()
{
    motor(-150, 100);
}

void big_turn_right()
{
    motor(90, -140);
}

void stop()
{
    motor(0, 0);
}

void re_turn_left()
{
    motor(-145, 100);
}
void re_turn_right()
{
    motor(110, -140);
}

void motor(int speedL, int speedR)
{
    // 判斷speedL跟speedR正負號決定方向
    digitalWrite(sL, speedL < 0 ? LOW : HIGH); // 左邊馬達方向設定
    digitalWrite(sR, speedR < 0 ? LOW : HIGH); // 右邊馬達方向設定

    analogWrite(mL, abs(speedL)); // 左邊馬達速度設定
    analogWrite(mR, abs(speedR)); // 右邊馬達速度設定
}