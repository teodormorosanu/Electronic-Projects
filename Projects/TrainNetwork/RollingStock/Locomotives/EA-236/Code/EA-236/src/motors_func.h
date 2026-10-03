#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --------------------------------------------------------------------------
// MOTORS SETUP
// --------------------------------------------------------------------------

/**
 * @brief Configures PWM channels and attaches them to motor driver pins.
 */
void motors_setup();

// --------------------------------------------------------------------------
// MOTORS FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Sets the motors PWM in different configurations.
 *
 * @param direction Train direction. ('F' = forward, 'B' = backward,
 *                                    'S' = brake)
 * @param throttle Motors throttle. (0 - 1023, Defaults to 0) 
 *
 * @return Returns -1 if direction is invalid, else it returns 0.
 */
int8_t set_motors(char direction, int16_t throttle = 0);

/**
 * @brief Testing the motors functionality.
 *
 * @param throttle Motors throttle. (0 - 1023)
 * @param delay_ms Apply a delay after testing a motor. (Defaults to 0)
   WARNING: Use with caution and avoid loop()!
 */
void test_motors(int16_t throttle = 0, int32_t delay_ms = 0);