#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --- I2C Protocol ---
#include <Wire.h>

// --------------------------------------------------------------------------
// GYROSCOPE SETUP
// --------------------------------------------------------------------------

/**
 * @brief Configures the I2C bus and initializes the MPU gyroscope.
 *
 * @returns Returns true if the setup was succesful, else it returns false.
 */
bool gyro_setup();

// --------------------------------------------------------------------------
// GYROSCOPE FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Reads gyro axes values.
 *
 * @param gyro_out Reference to where the axes values will be stored.
 *
 * @returns Returns true if the gyroscope updated, else it returns false.
 */
void read_gyro(float* gyro_out);