#include "Matrix16.h"

// #define _CATEDS_My_Board_Is_Designed_Wrongly_

namespace DT5EDU {

void matrix_16::SPI_write(uint16_t data) {
    #ifdef _Matrix_16_Using_Simulated_SPI_
    uint16_t delay = 1.e9/SPI_Freq/2;
    for (int i=15; i>=0; i--) {
        mosi = (data >> i) & 0x1;
        wait_ns(delay);
        clk = 1;
        wait_ns(delay);
        clk = 0;
    }
    #else
    spi.write(data);
    #endif
}

void matrix_16::Transmit_ForAll(Address addr, uint8_t data) {
    uint16_t temp = (addr << 8) | data;
    cs = 0;
    for (int i=0; i<4; i++)
        SPI_write(temp);
    cs = 1;
}

void matrix_16::Transmit(uint64_t data) {
    cs = 0;
    SPI_write(data>>48);
    SPI_write((data>>32) & 0xFFFF);
    SPI_write((data>>16) & 0xFFFF);
    SPI_write(data & 0xFFFF);
    cs = 1;
}

void matrix_16::Transmit(const array<pair<Address,uint8_t>,4> &operations) {
    cs = 0;
    for (auto &[addr,data]: operations)
        SPI_write((addr << 8) | data);
    cs = 1;
}

void matrix_16::Transmit(uint8_t indexOfSubmatrix, Address addr, uint8_t data) {
    cs = 0;
    SPI_write((addr << 8) | data);
    for (int i=0; i<indexOfSubmatrix; i++)
        SPI_write(0);
    cs = 1;
}

matrix_16 & matrix_16::SendBuffer(void) {
    for (int i=0; i<8; i++) {
        cs = 0;
#ifdef _CATEDS_My_Board_Is_Designed_Wrongly_
        SPI_write(((i+1) << 8) | (buffer[i+8] & 0xFF) >> 1 | (buffer[i+8] & 0x1) << 7);
        SPI_write(((i+1) << 8) | (buffer[i+8] >> 9) | (buffer[i+8] & 0x0100) >> 1) ;
        SPI_write(((i+1) << 8) | (buffer[i] & 0xFF) >> 1 | (buffer[i] & 0x1) << 7);
        SPI_write(((i+1) << 8) | (buffer[i] >> 9) | (buffer[i] & 0x0100) >> 1);
#else
        SPI_write(((i+1) << 8) | (buffer[i+8] & 0xFF));
        SPI_write(((i+1) << 8) | (buffer[i+8] >> 8));
        SPI_write(((i+1) << 8) | (buffer[i] & 0xFF));
        SPI_write(((i+1) << 8) | (buffer[i] >> 8));
#endif
        cs = 1;
    }
    return *this;
}

matrix_16 & matrix_16::Set_MatrixTest(bool mode) {
    Transmit_ForAll(DisplayTest,mode);
    inDisplayTest = mode;
    return *this;
}

matrix_16 & matrix_16::Set_Shutdown(bool mode) {
    Transmit_ForAll(Shutdown,!mode);
    inShutdown = mode;
    return *this;
}

matrix_16 & matrix_16::Set_Intensity(float intensity) {
    uint8_t value = 0b1111 * intensity;
    Transmit_ForAll(Intensity,value);
    return *this;
}

matrix_16 & matrix_16::InitSetting(void) {
    Transmit_ForAll(DecodeMode);
    Set_MatrixTest(false);
    Set_Shutdown(false);
    Set_Intensity();
    Transmit_ForAll(ScanLimit,0b111);
    return *this;
}

matrix_16 & matrix_16::operator<<(const frame &data) {
    buffer = data;
    return SendBuffer();
}

matrix_16::frame changeFrame(const matrix_16::frame &data, int operation) {
    matrix_16::frame RetVal = {0};
    switch (operation) {
    case Clkwse_0: RetVal = data; break;
    case Clkwse_90:
        for (int row=0; row<16; row++)
            for (int i=0; i<16; i++)
                RetVal[15-i] |= ((data[row] >> i) & 0x1) << row;
        break;
    case Clkwse_180:
        for (int row=0; row<16; row++) {
            auto val = data[row];
            for (int i=0; i<16; i++) {
                RetVal[15-row] <<= 1;
                RetVal[15-row] |= val & 0x1;
                val >>= 1;
            }
        }
        break;
    case Clkwse_270:
        for (int row=0; row<16; row++)
            for (int i=0; i<16; i++)
                RetVal[i] |= ((data[row] >> i) & 0x1) << (15-row);
        break;
    case Mirror_Hori:
        for (int row=0; row<16; row++) {
            auto val = data[row];
            for (int i=0; i<16; i++) {
                RetVal[row] <<= 1;
                RetVal[row] |= val & 0x1;
                val >>= 1;
            }
        }
        break;
    case Mirror_Verti:
        for (int i=0; i<16; i++)
            RetVal[15-i] = data[i];
        break;
    default: 
        RetVal = data;
        break;
    };
    return RetVal;
}

}