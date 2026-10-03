#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --------------------------------------------------------------------------
// LEDS SETUP
// --------------------------------------------------------------------------

/**
 * @brief Configures the LEDs pins mode.
 */
void leds_setup();

// --------------------------------------------------------------------------
// LEDS FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Sets the light LEDs in different configurations.
 *
 * @param direction Lights placement. ('F' = forward, 'B' = backward,
 *                                     'A' = all)
 * @param mode Lights power on/off. (HIGH = on, LOW = off)
 * @param delay_ms Apply a delay after setting the lights. (Defaults to 0)
   WARNING: Use ONLY in setup()!
 *
 * @return Returns -1 if direction is invalid, else it returns 0.
 */
int8_t set_lights(char direction, uint8_t mode, uint32_t delay_ms = 0);

/**
 * @brief Sets the beams LEDs in different configurations.
 *
 * @param direction Beams placement. ('F' = forward, 'B' = backward,
 *                                    'A' = all)
 * @param mode Beams power on/off. (HIGH = on, LOW = off)
 * @param delay_ms Apply a delay after setting the beams. (Defaults to 0)
   WARNING: Use ONLY in setup()!
 *
 * @return Returns -1 if direction is invalid, else it returns 0.
 */
int8_t set_beams(char direction, uint8_t mode, uint32_t delay_ms = 0);