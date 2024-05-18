#ifndef _CATEDS_MS_ADXL345_GY291_
#define _CATEDS_MS_ADXL345_GY291_

#include "mbed.h"
#include "cstdint"

#define _ADXL345_Using_Simulated_SPI_

namespace DT5EDU {

class ADXL345 {
private:
#ifdef _ADXL345_Using_Simulated_SPI_
    DigitalOut mosi, clk;
    DigitalIn miso;
#else
    SPI spi;
#endif
    bool cpol = 1, cpha = 1;
    DigitalOut cs;
    InterruptIn int1, int2;
    int Freq = 1000000;

    enum Address {
        THRESH_TAP = 0x1D,
        POWER_CTL = 0x2D,
        OFSX = 0x1E, OFSY = 0x1F, OSFZ = 0x20,
        INT_ENABLE = 0x2E, INT_MAP = 0x2F,
        DATA_FORMAT = 0x31,
        DATAX0 = 0x32, DATAX1 = 0x33, DATAY0 = 0x34, DATAY1 = 0x35,
        DATAZ0 = 0x36, DATAZ1 = 0x37,
        FIFO_CTL = 0x38, FIFO_STATUS = 0x39
    };

    void SPI_Write_16(uint16_t data);
    void SPI_Write_16(Address addr, uint8_t data);
    uint8_t SPI_Write_8_Read_8(uint8_t data);
    uint8_t SPI_Write_8_Read_8(Address addr);
    void Transmit_Write_16(Address addr, uint8_t data);
    uint8_t Transmit_Read_8(Address addr);
    void Transmit_Read_Data(void);

public:
    struct ADXL345_Data {
        int16_t x;
        int16_t y;
        int16_t z;
    } data;

    ADXL345(PinName spi_mosi, PinName spi_miso, PinName spi_clk, PinName spi_cs, 
            PinName int_1 = NC, PinName int_2 = NC):
#ifdef _ADXL345_Using_Simulated_SPI_
            mosi(spi_mosi), miso(spi_miso), clk(spi_clk), cs(spi_cs),
            int1(int_1), int2(int_2) {
        cs = 1;
        clk = cpol;
        data = {0,0,0};
        InitSetting();
        // Calibration();
    }
#else
            spi(spi_mosi, spi_miso, spi_clk), cs(spi_cs), int1(int_1), int2(int_2) {
        //TODO
    }
#endif
    ~ADXL345() {}

    void InitSetting(void);
    // void Calibration(void);
    ADXL345 & ReadData(void);
};

}
using DT5EDU::ADXL345;

#endif //_CATEDS_MS_ADXL345_GY291_
