#include "leds_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Hardware ---
// Swapped the red pair so the same i % 2 logic reverses forward and
// backward lights color with direction
uint8_t leds_pins[] = {
	FRONT_WHITE_LIGHTS, BACK_WHITE_LIGHTS,
	BACK_RED_LIGHTS, FRONT_RED_LIGHTS,
	FRONT_BEAMS, BACK_BEAMS
};

// --------------------------------------------------------------------------
// LEDS SETUP IMPLEMENTATION
// --------------------------------------------------------------------------

void leds_setup() {
	for (uint8_t i = 0; i < sizeof(leds_pins); i++) {
		pinMode(leds_pins[i], OUTPUT);
	}

	// Blinking sequence to visually indicate setup is done
	for (uint8_t i = 0; i < 3; i++) {
		set_beams('A', HIGH);
		set_lights('A', HIGH, 500);
		
		set_beams('A', LOW);
		set_lights('A', LOW, 500);
	}
}

// --------------------------------------------------------------------------
// LEDS FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

int8_t leds_range(char direction, uint8_t start_i, uint8_t end_i, 
				  uint8_t mode, uint32_t delay_ms) {
	// Power leds based on direction, index and mode (on/off)
	for (uint8_t i = start_i; i < end_i; i++) {
		switch (direction) {
			case 'F':
				digitalWrite(leds_pins[i], (0 == i % 2) ? mode : LOW);
				break;
			case 'B':
				digitalWrite(leds_pins[i], (0 != i % 2) ? mode : LOW);
				break;
			case 'A':
				digitalWrite(leds_pins[i], mode);
				break;
			default:
				return -1;
		}
	}

	// Applying a delay after powering the leds (ONLY IN SETUP!)
	if (0 < delay_ms) {
		delay(delay_ms);
	}
	return 0;
}

int8_t set_lights(char direction, uint8_t mode, uint32_t delay_ms) {
	return leds_range(direction, 0, sizeof(leds_pins) - 2, mode, delay_ms);
}

int8_t set_beams(char direction, uint8_t mode, uint32_t delay_ms) {
	return leds_range(direction, 4, sizeof(leds_pins), mode, delay_ms);
}