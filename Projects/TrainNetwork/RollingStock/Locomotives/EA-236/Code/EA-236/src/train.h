#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --------------------------------------------------------------------------
// EXTERN GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Locomotive Control ---
extern bool lights; // Train lights mode (true = on, false = off)
extern bool beams; // Train beams mode (true = on, false = off)

extern bool mapping; // Track mapping trigger (true = on, false = off)
extern bool reading; // Track RFID read trigger (true = on, false = off)

extern bool gyro; // Train gyroscope mode (true = on, false = off) 

extern bool motors_on; // Train motors mode (true = on, false = off)
extern bool horn; // Train horn mode (true = on, false = off)

// --- Driving ---
extern char direction; // Train direction ('F' = forward, 'B' = backward,
                                       // 'S' = brake)
extern int16_t target_throttle; // Train desired motor speed

// --- States ---
extern bool changing_direction; // Indicates if the train is braking
extern char pending_direction; // Wanted direction after throttle reaches 0

// --------------------------------------------------------------------------
// TRAIN FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Drives the train with smooth acceleration and safe reversing.
 */
void train_drive();

/**
 * @brief Controls the train leds.
 */
void train_leds();

/**
 * @brief Controls the train auto track mapping feature.
 */
void train_auto_mapping();

/**
 * @brief Controls the train sounding feature.
 */
void train_sound();

/**
 * @brief Controls all the train functions. (ex: driving and leds).
 */
void control_train();