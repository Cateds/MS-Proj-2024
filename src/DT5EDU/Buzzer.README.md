->pin = pin;
    pinMode(pin, OUTPUT);
}

/**
 * @brief Plays a tone on the buzzer.
 *
 * This function generates a tone of the specified frequency and duration on the buzzer.
 *
 * @param frequency The frequency of the tone to be played.
 * @param duration The duration of the tone in milliseconds.
 */
void Buzzer::playTone(int frequency, int duration) {
    // Generate the tone on the buzzer
    tone(pin, frequency, duration);
}

/**
 * @brief Stops the buzzer from playing any tone.
 *
 * This function stops the buzzer from playing any tone.
 */
void Buzzer::stopTone() {
    // Stop the tone on the buzzer
    noTone(pin);
}
