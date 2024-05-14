/**
 * @file Buzzer.h
 * @author CATEDS
 * @brief A library for buzzer to play music.
 * @version 1.4
 * @date 2024-05-14
 * 
 * @copyright Copyright (c) 2024 by DT5EDU Org.
 * 
 * @namespace DT5EDU
 * @class buzzer
 */

#ifndef _CATEDS_MS_Buzzer_
#define _CATEDS_MS_Buzzer_

#include "mbed.h"
#include <unordered_map>
#include <vector>

namespace DT5EDU {

/**
 * @brief The enum class for the tunes of the notes.
 */
enum class Tune {
    C3,C3s,D3,D3s,E3,F3,F3s,G3,G3s,A3,A3s,B3,
    C4,C4s,D4,D4s,E4,F4,F4s,G4,G4s,A4,A4s,B4,
    C5,C5s,D5,D5s,E5,F5,F5s,G5,G5s,A5,A5s,B5,
    Pause,Sustain
};

/**
 * @brief Use the STL in standard library to translate the notes 
 *        into the corresponding frequency.
 */
extern std::unordered_map<Tune,int> Tune2Freq;

/**
 * @brief The class for a buzzer module, and it's driven by an PWM output
 *        of your board.
 */
class buzzer {
private:
    PwmOut IO;
public:

    /**
     * @brief Construct a new buzzer object
     * 
     * @param Pin_PWMOUT    The Pin to control the sound made by the buzzer.
     *                      CAUTION: The pin should be capable for PWM output!
     */
    buzzer(PinName Pin_PWMOUT): IO(Pin_PWMOUT) {
        IO.write(0.f);
    }
    ~buzzer() {}

    /**
     * @brief Set the sould output of the buzzer according to the tune.
     * 
     * @param tune_input    The tune you want to play.
     * @return buzzer&      The reference of object itself
     */
    buzzer & SetTune(Tune tune_input = Tune::Pause);

    /**
     * @brief Set the sould output of the buzzer according to the tune.
     *        However, this function uses the operator <<, which looks cool.
     * @param tune_input    The tune you want to play.
     * @return buzzer&      The reference of object itself
     */
    buzzer & operator<<(Tune tune_input) {
        return SetTune(tune_input);
    }

    /**
     * @brief Play the music according to the array you input and the speed you want.
     * 
     * @param Music         I recommend you to see the README.md first. 
     *                      Therefore you would know how to write the input.
     * @param NotePerMin    The number of the note you want it to play every minute.
     *                      The function use the number to decide the base speed of playing music.
     * @return buzzer&      The reference of the object itself.
     */
    buzzer & PlayMusic(const vector<Tune> &Music, float NotePerMin);

    /**
     * @brief Play the music according to the array you input and the speed you want.
     * 
     * @param Music         I recommend you to see the README.md first. 
     *                      Therefore you would know how to write the input.
     * @param NotePerMin    The number of the note you want it to play every minute.
     *                      The function use the number to decide the base speed of playing music.
     * @return buzzer&      The reference of the object itself.
     */
    buzzer & PlayMusic(const vector<pair<Tune,float>> &Music, float NotePerMin);
    
    /**
     * @brief 《小星星》完整歌曲
     * 
     * @return buzzer& The reference of the object itself
     */
    buzzer & DemoMusic_1(void);

    /**
     * @brief 《春日影》，但是只有前11个小节
     * 
     * @return buzzer& The reference of the object itself
     */
    buzzer & DemoMusic_2(void);
};

}

using DT5EDU::buzzer;

#endif //_CATEDS_MS_Buzzer_