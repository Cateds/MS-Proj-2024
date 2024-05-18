#include "ADXL345.h"

namespace DT5EDU {

void ADXL345::SPI_Write_16(uint16_t data) {
    uint8_t delay = 1.e9/Freq/2;
    for (int i=15; i>=0; i--) {
        mosi = (data >> i) & 0x1;
        wait_ns(delay);
        clk = !clk;
        wait_ns(delay);
        clk = !clk;
    }
}

void ADXL345::SPI_Write_16(Address addr, uint8_t data) {
    SPI_Write_16((addr << 8) | data);
}

uint8_t ADXL345::SPI_Write_8_Read_8(uint8_t data) {
    uint8_t delay = 1.e9/Freq/2;
    uint8_t RetVal = 0;
    for (int i=7; i>=0; i--) {
        mosi = (data >> i) & 0x1;
        wait_ns(delay);
        clk = !clk;
        wait_ns(delay);
        clk = !clk;
    }
    for (int i=0; i<8; i++) {
        wait_ns(delay);
        clk = !clk;
        RetVal <<= 1;
        wait_ns(delay);
        clk = !clk;
        RetVal |= miso;
    }
    return RetVal;
}

uint8_t ADXL345::SPI_Write_8_Read_8(Address addr) {
    uint8_t temp = addr | (0x1 << 8);
    uint8_t delay = 1.e9/Freq/2;
    uint8_t RetVal = 0;
    for (int i=7; i>=0; i--) {
        mosi = (temp >> i) & 0x1;
        wait_ns(delay);
        clk = !clk;
        wait_ns(delay);
        clk = !clk;
    }
    for (int i=0; i<8; i++) {
        wait_ns(delay);
        clk = !clk;
        RetVal <<= 1;
        wait_ns(delay);
        clk = !clk;
        RetVal |= miso;
    }
    return RetVal;
}

void ADXL345::Transmit_Read_Data(void) {
    uint8_t temp = DATAX0 | 0b11000000;
    uint8_t delay = 1.e9/Freq/2;
    cs = 0;
    for (int i=7; i>=0; i--) {
        mosi = (temp >> i) & 0x1;
        wait_ns(delay); clk = !clk;
        wait_ns(delay); clk = !clk;
    }
    temp = 0;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.x = temp;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.x |= temp << 8;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.y = temp;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.y |= temp << 8;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.z = temp;
    for (int i=0; i<16; i++) {
        wait_ns(delay); clk = !clk; temp <<= 1;
        wait_ns(delay); clk = !clk; temp |= miso;
    }
    data.z |= temp << 8;
    cs = 1;

    data.x &= 0x8FFF;
    data.y &= 0x8FFF;
    data.z &= 0x8FFF;
}

void ADXL345::Transmit_Write_16(Address addr, uint8_t data) {
    cs = 0;
    SPI_Write_16(addr,data);
    cs = 1;
}

uint8_t ADXL345::Transmit_Read_8(Address addr) {
    cs = 0;
    auto RetVal = SPI_Write_8_Read_8(addr);
    cs = 1;
    return RetVal;
}

void ADXL345::InitSetting(void) {
    Transmit_Write_16(DATA_FORMAT, 0x0B);
    Transmit_Write_16(POWER_CTL, 0x08);
    Transmit_Write_16(INT_ENABLE, 0x80);
    Transmit_Write_16(INT_MAP, 0x00);
}

// void ADXL345::Calibration(void) {
//     float ave_x = 0, ave_y = 0, ave_z = 0;
//     uint8_t N = 10;
//     for (int i=0; i<N; i++) {
//         Transmit_Read_Data();
//         ave_x += data.x/(float)N;
//         ave_y += data.y/(float)N;
//         ave_z += data.z/(float)N;
//     }
//     // Transmit_Write_16(OFSX, -(ave_x/4));
// }

ADXL345 & ADXL345::ReadData(void) {
    Transmit_Read_Data();
    return *this;
}

}