#pragma once

// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Core & Utilities ---
#include <Arduino.h> // Arduino framework

// --- SPI Protocol ---
#include <SPI.h>

// --- RFID ---
#include <MFRC522.h>

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

enum TrackType : uint8_t {
    EMPTY_TAG = 0,
    STRAIGHT,
    DIAMOND_90,
    RERAILER,
    CURVED_5,
    CURVED_10,
    SWITCH_STRAIGHT_2,
    SWITCH_CURVED_2,
    UNDEFINED = 255
};

struct Track {
    uint64_t uid; // The track tag UID
    TrackType type; // The track type (ex: STRAIGHT, DIAMOND_90, RERAILER)
    char property; // Any track propery (ex: 'S' - straight, 'L' - left,
                                          // 'R' - right)
};

extern Track track_sequence[2048];
extern uint16_t sequence_index;

// --------------------------------------------------------------------------
// RFID SETUP
// --------------------------------------------------------------------------

/**
 * @brief Configures the SPI bus and initializes the RFID reader.
 */
void rfid_setup();

// --------------------------------------------------------------------------
// RFID FUNCTIONS
// --------------------------------------------------------------------------

/**
 * @brief Reads a track code from a tag's memory page.
 *
 * @param track_code_out Reference to where the track code will be stored.
 * @param uid Reference to where the tag ID will be stored.
 * @param page The tag's memory page to read from. (Defaults to page 4)
 *
 * @return Returns true if the read succeeded, else it returns false.
 */
bool read_track(uint8_t *track_code_out, uint64_t *uid_out, byte page = 4);

/**
 * @brief Checks if a piece of track is already mapped.
 *
 * @param uid The track UID that the check is based on.
 *
 * @return Returns true if the track is mapped already, else it returns false.
 */
bool is_track_mapped(uint64_t uid);

/**
 * @brief Checks if a piece of track is curved.
 *
 * @param type The track type that the check is based on.
 *
 * @return Returns true if the track is curved, else it returns false.
 */
bool is_curved_type(TrackType type);

/**
 * @brief Stores data inside the track sequence buffer.
 *
 * @param uid The track UID that is stored.
 * @param type The track type that is stored.
 * @param property The track property that is stored.
 *
 * @returns Returns true if there is available space in the track sequence,
            else it returns false;
 */
bool store_track_data(uint64_t uid, TrackType type, char property);