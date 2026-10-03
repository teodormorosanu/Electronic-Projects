#include "sound_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Hardware ---
DFRobotDFPlayerMini df_player; // DFPlayer Mini instance

// --------------------------------------------------------------------------
// SOUND SETUP IMPLEMENTATION
// --------------------------------------------------------------------------

bool sound_setup() {
    // Initialising sound with UART0
    Serial.begin(9600);

    // Checking if the DFPlayer Mini is working properly
    if (!df_player.begin(Serial)) {
        return false;
    }

    play_sound(SETUP, 15);
    return true;
}

// --------------------------------------------------------------------------
// SOUND FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

void play_sound(uint8_t index, uint8_t volume) {
    // Setting sound volume
    df_player.volume(volume);

    // Playing the sound with the given index
    df_player.play(index);
}

void stop_sound() {
    df_player.stop();
}

void set_volume(uint8_t volume) {
    // Setting sound volume
    df_player.volume(volume);
}

bool is_sound_finished() {
    // Checking DFPlayer messages
    if (df_player.available()) {
        // Checking if current sound ended
        if (df_player.readType() == DFPlayerPlayFinished) {
            df_player.read(); 
            return true;
        }
    }
    return false;
}