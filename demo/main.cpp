#include "mbed.h"

#define CENTER_BASE 1500

BufferedSerial pc(USBTX, USBRX);
DigitalIn encoder(D10);
PwmOut servo(D11);

Timer t;
Ticker encoder_ticker;

volatile int steps;
volatile int last;

void servo_control(int speed) {
    if (speed > 200)       speed = 200;
    else if (speed < -200) speed = -200;

    servo = (CENTER_BASE + speed) / 20000.0f;
}

void encoder_control() {
    int value = encoder;
    if (!last && value) steps++;
    last = value;
}

int main() {
   pc.set_baud(9600);
   encoder_ticker.attach(&encoder_control, .01);
   servo.period_ms(20);

    float time;

   servo_control(100);              // 表示開始
   ThisThread::sleep_for(1000ms);
   servo_control(0);                // 檢驗速度為0時為靜止
   ThisThread::sleep_for(5000ms);
   while (1) {
      servo_control(-25);           // 逆時針5cm/sec
      steps = 0;                    // 計數歸0
      t.reset();                    // 計時歸0
      t.start();                    // 開始計時
      ThisThread::sleep_for(5000ms);    // 持續5秒
      time = t.read();                  // 讀取、紀錄計時時間
      printf("%1.3f\r\n", (float)steps * 6.5 * 3.14 / 32 / time);   // 換算速率

      servo_control(180);           // 順時針8cm/sec
      steps = 0;                    // 計數歸0
      t.reset();                    // 計時歸0
      t.start();                    // 開始計時
      ThisThread::sleep_for(8000ms);    // 持續8秒
      time = t.read();                  // 讀取、紀錄計時時間
      printf("%1.3f\r\n", (float)steps * 6.5 * 3.14 / 32 / time);   // 換算速率
   }
}