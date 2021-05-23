#include "mbed.h"

#define CENTER_BASE 1500

BufferedSerial pc(USBTX, USBRX);
DigitalIn encoder(D10);
PwmOut servo(D11);

Timer t;
Timer t2;
Ticker encoder_ticker;

volatile int steps;
volatile int last;

void servo_control(int speed) {
   if (speed > 200)       speed = 200;
   else if (speed < -200) speed = -200;

   servo=(CENTER_BASE + speed)/20000.0f;
}

void encoder_control() {
   int value = encoder;
   if(!last && value) steps++;
   last = value;
//    printf("steps: %d\r\n", steps);
}


int main() {

   pc.set_baud(9600);

   encoder_ticker.attach(&encoder_control, 10ms);

   servo.period_ms(20);

   while(1) {

        //TODO: revise this value according to your result
        // servo_control(36.88186489367398); // 7cm/sec
        servo_control(-27.604105331599662); // -5cm/sec
        steps = 0;
        t.reset();
        t.start();

        ThisThread::sleep_for(5000ms);

        float time = t.read();

        printf("clockwise steps: %d\r\n", steps);
        printf("speed: %1.3f\r\n\n", (float) steps * 6.5 * 3.14 / 32 / time);

        servo_control(41.433621149894336); //8cm/sec
        steps = 0;
        t2.reset();
        t2.start();

        ThisThread::sleep_for(5000ms);

        float time2 = t2.read();

        printf("counterclockwise steps: %d\r\n", steps);
        printf("speed: %1.3f\r\n\n", (float) steps * 6.5 * 3.14 / 32 / time2);

   }
}