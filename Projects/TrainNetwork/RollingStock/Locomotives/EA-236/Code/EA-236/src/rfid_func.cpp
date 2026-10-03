#include "rfid_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Hardware ---
MFRC522 rfid(RFID_SS_PIN, RFID_RST_PIN); // RFID module instance

// --- Track structure ---
Track track_sequence[TRACK_LENGTH]; // The track sequence (Number of track pieces)
uint16_t sequence_index = 0; // The track sequence index

// --------------------------------------------------------------------------
// RFID SETUP IMPLEMENTATION
// --------------------------------------------------------------------------

void rfid_setup() {
    // Remapping the SPI pins
    SPI.begin(RFID_SCK_PIN, RFID_MISO_PIN, RFID_MOSI_PIN, RFID_SS_PIN);

    // RFID initialisation
    rfid.PCD_Init();

    // Setting the RFID antenna gain to maximum
    rfid.PCD_SetAntennaGain(rfid.RxGain_max);
}

// --------------------------------------------------------------------------
// RFID FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

bool read_track(uint8_t *track_code_out, uint64_t *uid_out, byte page) {
    // Checking if a new tag is present and its serial can be read
    if (!rfid.PICC_IsNewCardPresent()) return false;
    if (!rfid.PICC_ReadCardSerial()) return false;

    // Converting the UID into a 64 bit number
    *uid_out = 0;
    for (byte i = 0; i < rfid.uid.size; i++) {
        *uid_out = (((*uid_out) << 8) | rfid.uid.uidByte[i]);
    }

    // The MIFARE_Read function requires a buffer of at least 18 bytes.
    // Even though NTAG/Ultralight pages are 4 bytes, the library reads 
    // 16 bytes (4 pages at once) plus 2 bytes for the CRC.
    byte buffer[18];
    byte buffer_size = sizeof(buffer);

    // Attempting to read from the specified page
    MFRC522::StatusCode status = 
        rfid.MIFARE_Read(page, buffer, &buffer_size);

    if (status == MFRC522::STATUS_OK) {
        // The track code is stored in the first byte of the requested page
        *track_code_out = buffer[0];
    }

    // Halting the tag and stopping encryption, ready for the next reading
    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    
    return (MFRC522::STATUS_OK == status);
}

bool is_track_mapped(uint64_t uid) {
    for (uint16_t i = 0; i < sequence_index; i++) {
        if (uid == track_sequence[i].uid) {
            return true;
        }
    }
    return false;
}

bool is_curved_type(TrackType type) {
    return CURVED_5 == type || CURVED_10 == type || SWITCH_CURVED_2 == type;
}

bool store_track_data(uint64_t uid, TrackType type, char property) {
    // Storing the track type (code) and UID
    if (TRACK_LENGTH <= sequence_index) {
        return false;
    }
    
    track_sequence[sequence_index].uid = uid;
    track_sequence[sequence_index].type = type;
    track_sequence[sequence_index].property = property;
    sequence_index++;
    return true;
}