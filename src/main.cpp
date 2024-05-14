#include "mbed.h"
#include "Buzzer.h"
#include "Matrix.h"

// using namespace DT5EDU;

/*
* @return the return value;
*/
int main(void) {

    // buzzer Bzr(D9);
    matrix Mtrx(D11,D13,A3);
    DigitalOut led(LED1);
    Mtrx.Test();
    matrix::screen TestScr = {
        0,0b100,0b1100010,0b10,
        0b10,0b1100010,0b100,0
    };
    Mtrx.SendBuffer();
    ThisThread::sleep_for(200ms);
    Mtrx << TestScr;
    // Bzr.DemoMusic_2();
    while (true) {
        led = !led;
        ThisThread::sleep_for(300ms);
    }
}