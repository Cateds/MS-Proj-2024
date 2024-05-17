#include "mbed.h"
#include "Buzzer.h"
#include "Matrix.h"
#include "Matrix16.h"

int main(void) {
    matrix_16 Mtrx(D11,D13,A3);
    Mtrx.ClearBuffer().SendBuffer();
    ThisThread::sleep_for(1s);
    Mtrx.HelloWorld();
    // Mtrx.test();
    while (true) {
        ThisThread::sleep_for(1s);
        Mtrx.buffer = changeFrame(Mtrx.buffer, DT5EDU::Clkwse_90);
        Mtrx.SendBuffer();
    }
}