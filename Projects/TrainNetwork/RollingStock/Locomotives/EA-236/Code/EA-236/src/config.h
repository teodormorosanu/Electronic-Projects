#pragma once

// --------------------------------------------------------------------------
// MEMORY ADDRESSES
// --------------------------------------------------------------------------

// --- Storage adresses ---
#define SSID_ADDRESS 0
#define PASSWORD_ADDRESS 40

// --------------------------------------------------------------------------
// PIN DEFINITIONS
// --------------------------------------------------------------------------

// --- LEDs Pins ---
#define FRONT_WHITE_LIGHTS 15
#define BACK_WHITE_LIGHTS 2
#define FRONT_RED_LIGHTS 13
#define BACK_RED_LIGHTS 23
#define FRONT_BEAMS 12
#define BACK_BEAMS 22

// --- RFID Pins ---
#define RFID_SS_PIN 17
#define RFID_SCK_PIN 5
#define RFID_MOSI_PIN 27
#define RFID_MISO_PIN 36
#define RFID_IRQ_PIN 35
#define RFID_RST_PIN -1

// --- Gyroscope Pins ---
#define MPU_SDA 16
#define MPU_SCL 14
#define MPU_ADDRESS 0x68

// --- DFPlayer Pins ---
#define DF_TX 3
#define DF_RX 1

// --- Motor Driver Pins (Front) ---
#define FRONT_MOTOR_1_FORWARD 26
#define FRONT_MOTOR_1_BACKWARD 25
#define FRONT_MOTOR_2_FORWARD 33
#define FRONT_MOTOR_2_BACKWARD 32

// --- Motor Driver Pins (Back) ---
#define BACK_MOTOR_1_FORWARD 18
#define BACK_MOTOR_1_BACKWARD 4
#define BACK_MOTOR_2_FORWARD 19
#define BACK_MOTOR_2_BACKWARD 21

// --------------------------------------------------------------------------
// PWM CONFIGURATION
// --------------------------------------------------------------------------

// --- PWM Channels ---
#define CH_FRONT_1A 0
#define CH_FRONT_1B 1
#define CH_FRONT_2A 2
#define CH_FRONT_2B 3
#define CH_BACK_1A 4
#define CH_BACK_1B 5
#define CH_BACK_2A 6
#define CH_BACK_2B 7

// --- PWM Parameters ---
#define PWM_FREQUENCY 17000 // PWM frequency in Hz
#define PWM_RESOLUTION 10 // PWM resolution (0 - 1023)
#define RAMP_STEP 1 // Motor acceleration step
#define RAMP_DELAY 35 // Delay between acceleration / deceleration steps in ms

// --------------------------------------------------------------------------
// NETWORK CREDENTIALS
// --------------------------------------------------------------------------

// --- Access Point ---
#define AP_SSID "EA-236_Config" // ESP32 Access Point SSID 
#define AP_PASSWORD "teoelectric" // ESP32 Access Point password

// --- Over The Air ---
#define OTA_NAME "EA-236_OTA" // OTA name credential
#define OTA_PASSWORD "teoelectric" // OTA password credential

// --------------------------------------------------------------------------
// TRACK DETAILS
// --------------------------------------------------------------------------

// --- Track Configuration ---
#define TRACK_LENGTH 2048 // Track sequence lenght (maximum number of pieces)
#define CURVE_SAMPLE_MS 50 // Intregration time (based on the train speed)