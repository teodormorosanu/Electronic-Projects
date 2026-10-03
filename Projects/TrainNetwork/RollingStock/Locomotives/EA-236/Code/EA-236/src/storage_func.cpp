#include "storage_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// EEPROM & STORAGE FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

void eeprom_setup(unsigned size) {
    EEPROM.begin(size); // Allocating RAM buffer for EEPROM
}

void store_credentials(const char *ssid, const char *password) {
	if (PASSWORD_ADDRESS <= strlen(ssid)) {
		return;
	}
	EEPROM.writeString(SSID_ADDRESS, ssid); // Address 0: SSID
	EEPROM.writeString(PASSWORD_ADDRESS, password); // Address 40: Password
  	EEPROM.commit(); // Writing the buffer to flash memory
}