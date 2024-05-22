#include "mbed.h"
#include "DT5EDU/Buzzer.h"
#include "DT5EDU/Matrix.h"
#include "DT5EDU/Matrix16.h"
// #include "DT5EDU/ADXL345.h"
#include "Mbed/ADXL345.h"
#include "cstring"
#include "queue"

UnbufferedSerial srl(USBTX,USBRX,115200);
matrix_16 Mtrx(D11,D13,A3);
ADXL345 AccMtr(D2,A5,A1,D3);
buzzer Bzr(D9);
Thread thread;
int data[] = {0,0,0};
// BufferedSerial srl(A7,A2,115200);
matrix_16::frame haruhikage = {
    0x107E, 0x7C42, 0x3842, 0xFE7E,
    0x4442, 0xBA42, 0x387E, 0x0000,
    0x7208, 0x540C, 0xFA0C, 0x540A,
    0x7038, 0x2278, 0xAC30, 0x0000
};
matrix_16::frame umapoi = {
    0x7C28, 0x0428, 0x4428, 0x7E28,
    0x0228, 0xFA4A, 0x048E, 0x0000,
    0xF408, 0xB60C, 0xF40C, 0x560A,
    0xF438, 0xD478, 0xE630, 0x0000
};
matrix_16::frame basic_frame = {0};
using DT5EDU::Tune;
using DT5EDU::Tune2Freq;
vector<pair<Tune,float>> umapoi_music = {
    // 纯手工《马儿蹦跳传说》高潮部分第一个乐句？
    // 我们马P是这样的
    {Tune::G4,1},{Tune::G5,1},{Tune::G5,1},{Tune::G5,1},
    {Tune::G5,1},{Tune::F5,1},{Tune::E5,1},{Tune::C5,1},
    {Tune::Pause,1},{Tune::C5,1},{Tune::G5,2},
    {Tune::C5,1},{Tune::G5,2},{Tune::C5,1},
    {Tune::C5,1},{Tune::B4,1},{Tune::C5,1},{Tune::D5,3},{Tune::Pause,1.5},
    {Tune::G4,1},{Tune::D5,2},{Tune::D5,1},{Tune::G5,2},
    {Tune::B4,0.5},{Tune::C5,0.5},
    {Tune::D5,2},{Tune::C5,2},{Tune::B4,2},{Tune::C5,2},
    {Tune::Pause,1},{Tune::E5,1},{Tune::C5,1},{Tune::A4,4},
    {Tune::A4,1},{Tune::G4,1},{Tune::D5,2},{Tune::D5,4},{Tune::Pause,2},
    {Tune::D5,5},{Tune::E5,1},{Tune::F5,1},{Tune::E5,6}
};

void bzr_thread(void) {
    basic_frame = haruhikage;
    ThisThread::sleep_for(500ms);
    Bzr.DemoMusic_2();
    ThisThread::sleep_for(500ms);
    basic_frame = umapoi;
    ThisThread::sleep_for(500ms);
    Bzr.PlayMusic(umapoi_music,200);
    ThisThread::sleep_for(500ms);
    basic_frame.fill(0);
}

int main(void) {
    srl.attach([](void) {});
    srl.format();
    Mtrx.ClearBuffer().SendBuffer();
    Mtrx.Set_Intensity(0.25);
    Mtrx.HelloWorld();
    AccMtr.setPowerControl(0x00);
    AccMtr.setDataFormatControl(0x0B);
    AccMtr.setDataRate(ADXL345_800HZ);
    AccMtr.setPowerControl(0x08);
    ThisThread::sleep_for(1s);
    // Mtrx.test();
    char temp[30] = {0};  
    int8_t r = 0;

    thread.start(bzr_thread);
    while (true) {
        AccMtr.getOutput(data);
        r = (abs((int16_t)data[1])>abs((int16_t)data[0])) ? 
            (2 * ((int16_t)data[1]>=0)) : (2 * ((int16_t)data[1]<=0) + 1);
        sprintf(temp,"x,y,z:%d,%d,%d,%d\n",(int16_t)data[0],(int16_t)data[1],(int16_t)data[2],(int8_t)r);
        srl.write(temp,strlen(temp));
        Mtrx.buffer = changeFrame(basic_frame,(DT5EDU::Operation)r);
        Mtrx.SendBuffer();
        ThisThread::sleep_for(200ms);
    }
}
