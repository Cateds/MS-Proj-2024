#ifndef _CATEDS_MS_Matrix_16_
#define _CATEDS_MS_Matrix_16_

#include "mbed.h"
#include "cstdint"
#include "array"

#define _Matrix_16_Using_Simulated_SPI_

namespace DT5EDU {

class matrix_16 {
private:
    enum Address {
        NoOperation = 0b0000,
        Digit0 = 1, Digit1 = 2, Digit2 = 3, Digit3 = 4,
        Digit4 = 5, Digit5 = 6, Digit6 = 7, Digit7 = 8,
        DecodeMode  = 0b1001,
        Intensity   = 0b1010,
        ScanLimit   = 0b1011,
        Shutdown    = 0b1100,
        DisplayTest = 0b1111
    };


    int SPI_Freq = 1000000;

    #ifdef _Matrix_16_Using_Simulated_SPI_
    DigitalOut clk,mosi;
    #else
    SPI spi;
    #endif
    DigitalOut cs;

    bool inDisplayTest;
    bool inShutdown;
    
    void SPI_write(uint16_t data);
    void Transmit_ForAll(Address addr = NoOperation, uint8_t data = 0x0);
    void Transmit(uint64_t data);
    void Transmit(const array<pair<Address,uint8_t>,4> &operations);
    void Transmit(uint8_t indexOfSubmatrix, Address addr, uint8_t data);
public:
    array<uint16_t,16> buffer;
    using frame = array<uint16_t,16>;

    #ifdef _Matrix_16_Using_Simulated_SPI_
    matrix_16(PinName Matrix_IO_MOSI, PinName Matrix_IO_CLK, PinName Matrix_IO_CS):
            mosi(Matrix_IO_MOSI), clk(Matrix_IO_CLK), cs(Matrix_IO_CS) {
        cs = 1;
        clk = 0;
        buffer.fill(0);
        inDisplayTest = false;
        inShutdown = false;
        InitSetting();
    }
    #else
    matrix_16(PinName Matrix_SPI_MOSI, PinName Matrix_SPI_CLK, PinName Matrix_IO_CS):
            spi(Matrix_SPI_MOSI, NC, Matrix_SPI_CLK), cs(Matrix_IO_CS) {
        cs = 1;
        spi.frequency(SPI_Freq);
        spi.format(16,0);
        buffer.fill(0);
        inDisplayTest = false;
        inShutdown = false;
        InitSetting();
    }
    #endif
    ~matrix_16() {}

    matrix_16 & operator<<(const frame &data);
    matrix_16 & SendBuffer(void);
    matrix_16 & InitSetting(void);
    matrix_16 & Set_MatrixTest(bool mode);
    bool is_In_MatrixTest(void) { return inDisplayTest; }
    matrix_16 & Set_Shutdown(bool mode);
    bool is_In_Shutdown(void) { return inShutdown; }
    matrix_16 & ClearBuffer(void) { buffer.fill(0); return *this; }
    matrix_16 & Set_Intensity(float intensity = 1.f);
    matrix_16 & HelloWorld(void) {
        return *this << frame {
            0x0000, 0xA0A0, 0xEEAE, 0xAEAA,
            0xACAE, 0x0000, 0x8800, 0xABB0,
            0xAAA6, 0x53A0, 0x0000, 0x4404,
            0x5C32, 0x5402, 0x5D32, 0x0004
        };
    }
};

enum Rotation {
    Clkwse_0, Clkwse_90, Clkwse_180, Clkwse_270,
    Mirror_Verti, Mirror_Hori
};
matrix_16::frame changeFrame(const matrix_16::frame &data, Rotation operation);

}

using DT5EDU::matrix_16;
// using DT5EDU::Rotation;
using DT5EDU::changeFrame;

#endif //_CATEDS_MS_Matrix_16_
