#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --- DFPlayer Mini ---
#include <DFRobotDFPlayerMini.h> // DFPlayer Mini library

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

enum SoundIndex : uint8_t {
    HORN = 1,
    HORN_END,
    HUM,
    HUM_END,
    HORN_HUM,
    HORN_END_HUM,
    SETUP
};

// --------------------------------------------------------------------------
// SOUND SETUP
// --------------------------------------------------------------------------

/**
 * @brief Configures the sound and initializes the DFPlayer Mini.
 *
 * @returns Returns true if the setup was succesful, else it returns false.
 */
bool sound_setup();

// --------------------------------------------------------------------------
// SOUND FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Plays a sound based on an index.
 *
 * @param index The sound index on the Micro SD card.
 * @param volume The sound volume.
 */
void play_sound(uint8_t index, uint8_t volume);

/**
 * @brief Stops the current sound playing.
 */
void stop_sound();

/**
 * @brief Sets the current sound volume.
 *
 * @param volume The sound volume.
 */
void set_volume(uint8_t volume);

/**
 * @brief Checks if current sound has finished playing.
 *
 * @return Returns true if the sound ended, else it returns false.
 */
bool is_sound_finished();