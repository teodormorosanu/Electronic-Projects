#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework
#include <EEPROM.h>	// Non-volatile memory

// --------------------------------------------------------------------------
// EEPROM & STORAGE FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Initializes the EEPROM memory space.
 *
 * @param size Memory size.
 */
void eeprom_setup(unsigned size);

/**
 * @brief Saves Wi-Fi credentials to non-volatile memory.
 *
 * @param ssid Wi-Fi ssid.
 * @param password Wi-Fi password.
 */
void store_credentials(const char *ssid, const char *password);