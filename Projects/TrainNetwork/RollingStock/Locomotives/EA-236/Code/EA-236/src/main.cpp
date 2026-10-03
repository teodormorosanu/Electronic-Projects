// --------------------------------------------------------------------------
// LIBRARIES
// --------------------------------------------------------------------------

// --- Project Libraries
#include "config.h"
#include "storage_func.h"
#include "network_func.h"
#include "rfid_func.h"
#include "gyro_func.h"
#include "sound_func.h"
#include "motors_func.h"
#include "leds_func.h"
#include "train.h"

// --------------------------------------------------------------------------
// MAIN FUNCTIONS
// --------------------------------------------------------------------------

void setup() {
    sound_setup();
    leds_setup();
    motors_setup();
    rfid_setup();
    gyro_setup();
    eeprom_setup(512);
    create_ap();
    connect_wifi_eeprom();
    init_server();
}

void loop() {
    control_train();
    process_server();
}
