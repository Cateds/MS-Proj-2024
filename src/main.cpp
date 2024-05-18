#include "mbed.h"
#include "Buzzer.h"
#include "Matrix.h"
#include "Matrix16.h"
// #include "ADXL345.h"
#include "cstring"

UnbufferedSerial srl(USBTX,USBRX,115200);
matrix_16 Mtrx(D11,D13,A3);
// ADXL345 adxl(D2,A5,A1,D3);
// BufferedSerial srl(A7,A2,115200);

int main(void) {
    srl.attach([](void) {});
    srl.format();
    Mtrx.ClearBuffer().SendBuffer();
    Mtrx.Set_Intensity(0.25);
    Mtrx.HelloWorld();
    
    ThisThread::sleep_for(1s);
    // Mtrx.test();
    char temp[30];  
    while (true) {
        // ThisThread::sleep_for(200ms);
        ThisThread::sleep_for(20ms);
        // adxl.ReadData();
        Mtrx.buffer = changeFrame(Mtrx.buffer, DT5EDU::Clkwse_90);
        Mtrx.SendBuffer();
        // sprintf(temp,"data:%d,%d,%d\n",adxl.data.x,adxl.data.y,adxl.data.z);
        // srl.write(temp,strlen(temp));
    }
}

/*
 * Copyright (c) 2020 Arm Limited and affiliates.
 * SPDX-License-Identifier: Apache-2.0
 */

// #include "mbed.h"

// // Create a DigitalOutput object to toggle an LED whenever data is received.
// static DigitalOut led(LED1);

// // Create a UnbufferedSerial object with a default baud rate.
// static UnbufferedSerial serial_port(USBTX,USBRX);

// void on_rx_interrupt()
// {
//     char c;

//     // Toggle the LED.
//     led = !led;

//     // Read the data to clear the receive interrupt.
//     if (serial_port.read(&c, 1)) {
//         // Echo the input back to the terminal.
//         serial_port.write(&c, 1);
//     }
// }

// int main(void)
// {
//     // Set desired properties (9600-8-N-1).
//     serial_port.baud(115200);
//     serial_port.format(
//         /* bits */ 8,
//         /* parity */ SerialBase::None,
//         /* stop bit */ 1
//     );

//     // Register a callback to process a Rx (receive) interrupt.
//     serial_port.attach(&on_rx_interrupt, SerialBase::RxIrq);
// }