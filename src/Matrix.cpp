#include "Matrix.h"

namespace DT5EDU {

matrix & matrix::Transmit(Address addr, uint8_t data) {
    uint16_t temp = (addr << 8) + data;
    cs = 0;
    mtrx_spi.write(temp);
    cs = 1;
    return *this;
}

matrix & matrix::SendBuffer(void) {
    for (int i=1; i<=8; i++)
        Transmit(matrix::Address(i),buffer[i]);
    return *this;
}

matrix & matrix::Set_Intensity(float intensity) {
    uint8_t value = 0b1111 * intensity;
    Transmit(Intensity,value);
    return *this;
}

matrix & matrix::Init_Setting(void) {
    Transmit(DecodeMode);
    Set_MatrixTest(false);
    Set_Shutdown(false);
    Set_Intensity();
    Transmit(ScanLimit,0b111);
    return *this;
}

matrix & matrix::Set_MatrixTest(bool mode) {
    Transmit(DisplayTest,mode);
    inDisplayTest = mode;
    return *this;
}

matrix & matrix::Set_Shutdown(bool mode) {
    Transmit(Shutdown,mode);
    inShutdown = mode;
    return *this;
}

inline matrix & matrix::ClearBuffer(void) {
    buffer.fill(0);
    return *this;
}

matrix & matrix::operator<<(const screen &data) {
    buffer = data;
    return SendBuffer();
}

}
