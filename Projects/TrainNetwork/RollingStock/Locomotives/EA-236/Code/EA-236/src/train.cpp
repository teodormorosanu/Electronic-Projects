#include "train.h"
#include "leds_func.h"
#include "motors_func.h"
#include "rfid_func.h"
#include "gyro_func.h"
#include "sound_func.h"
#include "network_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Locomotive Control ---
bool lights = false; // Train lights mode (true = on, false = off)
bool beams = false; // Train beams mode (true = on, false = off)

bool mapping = false; // Track mapping trigger (true = on, false = off)
bool reading = false; // Track RFID read trigger (true = on, false = off)

bool gyro = false; // Train gyroscope mode (true = on, false = off)

bool motors_on = false; // Train motors mode (true = on, false = off)
bool horn = false; // Train horn mode (true = on, false = off)

bool printed_once = false; // Text formatting boolean

// --- Driving ---
char direction = 'F'; // Train direction ('F' = forward, 'B' = backward,
                                       // 'S' = brake)
int16_t throttle = 0; // Train current motor speed (0 - 1023)
int16_t target_throttle = 0; // Train desired motor speed
uint32_t last_ramp = 0; // Timestamp for non-blocking acceleration (ms)

// --- Auto Mapping ---
uint64_t pending_uid; // The pending curve UID
TrackType pending_curve_type; // The pending curve type

// --- Gyroscope ---
float yaw_accumulator = 0; // Accumulator for MPU gyroscope yaw
uint32_t curve_start = 0; // Curve starting time
uint64_t gyro_on = 0; // Timer for gyroscope on time

// --- Sounding ---
static bool last_horn_state = false; // Last horn state for the horn sound
static bool last_motors_state = false; // Last motors state for the hum sound
static uint8_t current_sound = 0; // Current played sound
static uint8_t current_volume = 0; // Current sound volume
static uint64_t ignore_sounds = 0; // Ignoring sounds for some time (ms)

// --- States ---
bool detecting_curve = false; // Indicates if the train enters a curve
bool changing_direction = false; // Indicates if the train is braking
char pending_direction = 'F'; // Wanted direction after throttle reaches 0

// --------------------------------------------------------------------------
// TRAIN FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

void train_drive() {
    // Ensuring ramp updates only happen every RAMP_DELAY milliseconds
    uint32_t now = millis();
    if (RAMP_DELAY > now - last_ramp) return;
    last_ramp = now;

    int current_target = target_throttle;

    if (!motors_on || changing_direction) {
        current_target = 0; 
    }
    
    // Accelerating / decelerating state with smooth transition to a target
    if (throttle < current_target) {
        // Accelerating depending on the current throttle
        int step = (throttle < 675) ? (RAMP_STEP * 35) : RAMP_STEP;

        // Clamping up
        if (throttle + step > current_target) {
            throttle = current_target;
        } else {
            throttle += step;
        }
    } else if (throttle > current_target) {
        // Decelerating depending on the current throttle
        int step = (throttle < 500) ? (RAMP_STEP * 100) : RAMP_STEP * 2;

        // Clamping down
        if (throttle <= step || throttle - step < current_target) {
            throttle = current_target; 
        } else {
            throttle -= step;
        }
    }
    set_motors(direction, throttle);

    // Braking state triggered by direction change request
    if (changing_direction && throttle == 0) {
        direction = pending_direction;
        changing_direction = false;
    }
}

void train_leds() {
    set_beams(direction, beams);
    set_lights(direction, lights);
}

void train_auto_mapping() {
    if (!mapping) {
        printed_once = false;
        return;
    }

    if (!printed_once) {
        WebSerial.printf("Auto mapping sequence started...\n");
        printed_once = true;
    }

    // Starting the mapping sequence. This includes powering the motors,
    // getting the track type, adding to a buffer the track sequence,
    // checking for type of curve. [...]

    // Powering the motors
    //set_motors(direction, 800);

    // Getting the track type based on gyroscope data
    if (reading) {
        uint64_t track_uid; uint8_t track_code;
        
        if (read_track(&track_code, &track_uid)) {
            WebSerial.printf("Read track UID: %llu | code: %u\n", track_uid,
                                                                  track_code);
            if (track_code == 0 || track_code > 7) {
                WebSerial.printf("Auto mapping stopped. Invalid RFID!");
                mapping = false;
                return;
            }
            
            TrackType track_type = static_cast<TrackType>(track_code);
            gyro = false;
            
            if (!is_track_mapped(track_uid)) {
                if (is_curved_type(track_type)) {
                    // Detected curved piece and starts integrating yaw
                    detecting_curve = true;
                    curve_start = millis();
                    yaw_accumulator = 0;
                    pending_uid = track_uid;
                    pending_curve_type = track_type;
                    gyro = true;
                } else {
                    store_track_data(track_uid, track_type, 'S');
                }
            }
        }
    }

    if (gyro) {
        uint64_t current_time = millis();
        if (current_time - gyro_on >= 50) {
            float dt = (current_time - gyro_on) / 1000.0f;
            gyro_on = current_time;

            // Getting the axes values
            float gyro_axes[3] = { 0, 0, 0 };
            read_gyro(gyro_axes);

            if (detecting_curve) {
                // Simple integration
                yaw_accumulator += gyro_axes[2] * dt;

                if (current_time - curve_start >= CURVE_SAMPLE_MS) {
                    char curve = (yaw_accumulator > 0) ? 'L' : 'R';
                    store_track_data(pending_uid, pending_curve_type,
                                     curve);
                    WebSerial.printf("Curve direction: %c (yaw sum: %.2f)\n",
                                  curve, yaw_accumulator);
                    detecting_curve = false;
                }
            }
        }
    }
}

void train_sound() {
    if (millis() < ignore_sounds) {
        return; 
    }
    
    uint8_t target_sound = 0;
    uint8_t target_volume = 18;
    bool should_loop = false;

    if (horn) {
        if (!motors_on) {
            target_sound = HORN;
        } else {
            target_sound = HORN_HUM;
        }
        should_loop = true;
    }
    else if (!horn && last_horn_state) {
        if (!motors_on) {
            target_sound = HORN_END;
        } else {
            target_sound = HORN_END_HUM;
        }
    } else if (!motors_on && last_motors_state) {
        target_sound = HUM_END;
    } else {
        if (motors_on) {
            target_sound = HUM;
            should_loop = true;
        } else {
            target_sound = 0;
        }
    }
    
    last_horn_state = horn;
    last_motors_state = motors_on;
    
    if (target_sound != current_sound) {
        if (target_sound == 0) {
            stop_sound();
        } else {
            play_sound(target_sound, target_volume);
            
            if (target_sound == HORN_END || target_sound == HORN_END_HUM) {
                ignore_sounds = millis() + 350;
            } else if (target_sound == HUM_END) {
                ignore_sounds = millis() + 1400;
            }
        }
        current_sound = target_sound;
        current_volume = target_volume;
    } else {
        if (target_volume != current_volume) {
            set_volume(target_volume);
            current_volume = target_volume;
        }

        if (should_loop && current_sound != 0) {
            // Playing the sound again if it finished
            if (is_sound_finished()) {
                play_sound(current_sound, current_volume);
            }
        }
    }
}

void control_train() {
    train_drive();
    train_leds();
    train_auto_mapping();
    train_sound();
}