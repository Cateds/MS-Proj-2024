/**
 * @file Matrix.h
 * @author CATEDS
 * @brief A class designed for the 8*8 LED matrix module
 *        with the connection of SPI and drived by MAX7219
 * @version 0.1
 * @date 2024-05-14
 * 
 * @copyright Copyright (c) 2024
 * @namespace DT5EDU
 * @class matrix
 */

#ifndef _CATEDS_MS_Matrix_
#define _CATEDS_MS_Matrix_

#include "mbed.h"
#include "array"
#include "cstdint"

namespace DT5EDU {

/**
 * @brief A class designed for the 8*8 LED matrix module
 *        with the connection of SPI and drived by MAX7219
 */
class matrix {
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

    SPI mtrx_spi;
    DigitalOut cs;

    bool inDisplayTest;
    bool inShutdown;

    
    /**
     * @brief   The basic function to transmit the data to the slave device
     *          according to the input variables
     * 
     * @param addr  the address defined in enum Address
     * @param data  the data of the corresponding address
     * @return matrix&  the reference of the object itself
     */
    matrix & Transmit(Address addr = NoOperation, uint8_t data = 0x0);
public:
    array<uint8_t,8> buffer;

    /**
     * @brief Construct a new matrix object
     * 
     * @param Matrix_SPI_MOSI   Pin for MOSI port for SPI.
     * @param Matrix_SPI_CLK    Pin for CLK port for SPI.
     * @param Matrix_IO_CS      Pin for CS/SS port for SPI.
     */
    matrix( PinName Matrix_SPI_MOSI, PinName Matrix_SPI_CLK,  PinName Matrix_IO_CS ) : 
            mtrx_spi(Matrix_SPI_MOSI, NC, Matrix_SPI_CLK), cs(Matrix_IO_CS) {
        cs = 1;
        mtrx_spi.frequency(10000);
        mtrx_spi.format(16,0);
        buffer.fill(0);
        inDisplayTest = false;
        inShutdown = false;
        Init_Setting();
    }
    ~matrix() {}
    
    using screen = array<uint8_t,8>;
    matrix & operator<<(const screen &data);
    matrix & SendBuffer(void);
    matrix & Init_Setting(void);
    matrix & Set_MatrixTest(bool mode);
    bool is_In_MatrixTest(void) { return inDisplayTest; }
    matrix & Set_Shutdown(bool mode);
    bool is_In_Shutdown(void) { return inShutdown; }
    matrix & ClearBuffer(void);
    matrix & Set_Intensity(float intensity = 1.f);
    void Test(void) {cs = 1;}
};

}

using DT5EDU::matrix;

#endif //_CATEDS_MS_Matrix_