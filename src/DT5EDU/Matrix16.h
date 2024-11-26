/**
 * @file Matrix.h
 * @author CATEDS
 * @brief A class designed for the 16*16 LED matrix_8 module
 *        with the connection of SPI and drived by MAX7219
 *        The order of the four 8*8 led matrix should be:
 *          1 | 2
 *          --+--
 *          3 | 4
 * @version 0.1
 * @date 2024-05-17
 * 
 * @copyright Copyright (c) 2024 DT5EDU
 * @namespace DT5EDU
 * @class matrix_16
 */

#ifndef _CATEDS_MS_Matrix_16_
#define _CATEDS_MS_Matrix_16_

#include "mbed.h"
#include "cstdint"
#include "array"

/**
 * @brief It's a macro which decides how would you like to transmit the signal.
 *        If this macro is defined, then the program will use simulated SPI to transmit data.
 *        While if the macro is be commented or undefined, the program will use hardware SPI to do such things.
 *        More info would be in README.md
*/
#define _Matrix_16_Using_Simulated_SPI_

namespace DT5EDU {

/**
 * @brief A class designed for the 16*16 LED matrix_16 module
 *        with the connection of SPI and drived by MAX7219
 *        The order of the four 8*8 led matrix should be:
 *          1 | 2
 *          --+--
 *          3 | 4
 */
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
    //陈星昊

    /**
     * @brief The Frequency of the SPI Communication.
     *        You can rewrite this value to change the communication rate.
     */
    int SPI_Freq = 1000000;

#ifdef _Matrix_16_Using_Simulated_SPI_
    DigitalOut clk,mosi;
#else
    SPI spi;
#endif
    DigitalOut cs;

    bool inDisplayTest;
    bool inShutdown; 
    
    /**
     * @brief Write 16-bit data without changing CS
     * 
     * @param data  The 16-bit data you want to transmit.
     */
    void SPI_write(uint16_t data);
    /**
     * @brief send the address and data for all the 4 MAX7219 devices.
     *        CS would change automatically.
     * @param addr Address
     * @param data The Corresponding 8-bit data
     */
    void Transmit_ForAll(Address addr = NoOperation, uint8_t data = 0x0);
    /**
     * @brief Transmit 64-bit data by only ONCE.
     *        CS would change automatically.
     * @param data  The 64-bit data.
     */
    void Transmit(uint64_t data);
    /**
     * @brief Send the 4 set of operations to the 4 MAX7219 device.
     * 
     * @param operations The set of operations for the corresponding matrix 1-4
     */
    void Transmit(const array<pair<Address,uint8_t>,4> &operations);
    /**
     * @brief Send the operation to one particular submatrix.
     * 
     * @param indexOfSubmatrix the index of the target matrix, which has a range of [0,3]
     * @param addr Address
     * @param data The corresponding 8-bit data.
     */
    void Transmit(uint8_t indexOfSubmatrix, Address addr, uint8_t data);
public:
    array<uint16_t,16> buffer;

    /**
     * @brief Define the alias of std::array<uint16_t,16>
     * 
     */
    using frame = array<uint16_t,16>;

#ifdef _Matrix_16_Using_Simulated_SPI_
    /**
     * @brief Construct a new matrix_16 object
     *        CAUTION: You could choose the pins without SPI function.
     *                 The only need of the pin is supporting DigitalOut.
     * @param Matrix_IO_MOSI    Pin for MOSI port for simulated SPI.
     * @param Matrix_IO_CLK     Pin for CLK port for simulated SPI.
     * @param Matrix_IO_CS      Pin for CS/SS port for simulated SPI.
     */
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
    /**
     * @brief Construct a new matrix_16 object
     *        CAUTION: You should choose the pins with hardware SPI function.
     *                 Because you chose the hardware SPI choice.
     * @param Matrix_SPI_MOSI    Pin for MOSI port for SPI of the board.
     * @param Matrix_SPI_CLK     Pin for CLK port for SPI of the board.
     * @param Matrix_IO_CS       Pin for CS/SS port for SPI.
     */
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

    /**
     * @brief  Display one frame on the matrix_16. This function would send the buffer automatically.
     * 
     * @param data the frame of the display you want wo display in the type of array<uint16_t,16>
     * @return matrix_16&   The reference of the object itself.
     */
    matrix_16 & operator<<(const frame &data);
    /**
     * @brief Send the data in the array object buffer to the matrix
     * 
     * @return matrix_16&  The reference of the object itself.
     */
    matrix_16 & SendBuffer(void);
    /**
     * @brief Initialized the default settings of the matrix_16;
     * 
     * @return matrix_16&  The reference of the object itself.
     */
    matrix_16 & InitSetting(void);
    /**
     * @brief Set the DisplayTest mode of MAX7219. 
     * 
     * @param mode THe choice whether you want to turn on the DisplayTest mode. 
     *             true or 1: Turn on the DisplayTest mode. 
     *             false or 0: Turn off the DisplayTest mode. 
     * @return matrix_16&  The reference of the object itself.
     */
    matrix_16 & Set_MatrixTest(bool mode);
    /**
     * @brief Check if the matrix_16 is in DisplayTest mode.
     * 
     * @return whether the matrix_16 is in DisplayTest mode.
     * @retval true :   It is in DisplayTest mode.
     * @retval false :  It isn't in DisplayTest mode.
     */
    bool is_In_MatrixTest(void) { return inDisplayTest; }
    /**
     * @brief Set the Shutdown mode of MAX7219. 
     * 
     * @param mode THe choice whether you want to turn on the Shutdown mode. 
     *             true or 1: Turn on the Shutdown mode. 
     *             false or 0: Turn off the Shutdown mode. 
     * @return matrix_16&  The reference of the object itself.
     */  
    matrix_16 & Set_Shutdown(bool mode);
    /**
     * @brief Check if the matrix_16 is in Shutdown mode.
     * 
     * @return whether the matrix_16 is in Shutdown mode.
     * @retval true :   It is in Shutdown mode.
     * @retval false :  It isn't in Shutdown mode.
     */
    bool is_In_Shutdown(void) { return inShutdown; }
    /**
     * @brief clear the buffer array inside the object.
     *        This function won't send the buffer automatically.
     * 
     * @return matrix_16&  The reference of the object itself.
     */
    matrix_16 & ClearBuffer(void) { buffer.fill(0); return *this; }
    /**
     * @brief Set the intensity (or called brightness) of the matrix_16.
     * 
     * @param intensity The target brightness of the matrix_16.
     *                  The input value should be in the range of 1.0f and 0.0f
     * @return matrix_16&  The reference of the function itself.
     */
    matrix_16 & Set_Intensity(float intensity = 1.f);
    /**
     * @brief A test function to display a smile on your matrix_168 to test whether
     *         your harware could work. Therefore it's named as HelloWorld. 
     *        Actually it would do display "Hello Wor-ld :)" on the matrix.
     * @return matrix_16&  The reference of the object itself.
     */
    matrix_16 & HelloWorld(void) {
        return *this << frame {
            0x0000, 0xA0A0, 0xEEAE, 0xAEAA,
            0xACAE, 0x0000, 0x8800, 0xABB0,
            0xAAA6, 0x53A0, 0x0000, 0x4404,
            0x5C32, 0x5402, 0x5D32, 0x0004
        };
    }
};

/**
 * @brief Define the frame operations for the changeFrame function
 * 
 */
enum Operation {
    Clkwse_0, Clkwse_90, Clkwse_180, Clkwse_270,
    Mirror_Verti, Mirror_Hori
};
/**
 * @brief   This function would rotate or mirror the input frame and
 *          return the modified frame. The operation type is depending 
 *          on the parameter.
 * 
 * @param data          The raw frame you want to input.
 * @param operation     The Corresponding Operation for the frame.
 *                      The connection of the value and the operation is defined in the enum Operation.
 * @return matrix_16::frame     The modified frame
 */
matrix_16::frame changeFrame(const matrix_16::frame &data, int operation);

}

using namespace DT5EDU;

#endif //_CATEDS_MS_Matrix_16_
