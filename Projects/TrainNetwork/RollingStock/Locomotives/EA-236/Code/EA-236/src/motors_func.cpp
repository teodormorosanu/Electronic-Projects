#include "motors_func.h"
#include "network_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GLOBAL VARIABLES
// --------------------------------------------------------------------------

// --- Hardware ---
uint8_t motor_pins[] = {
	FRONT_MOTOR_1_FORWARD, FRONT_MOTOR_1_BACKWARD,
	FRONT_MOTOR_2_FORWARD, FRONT_MOTOR_2_BACKWARD,
	BACK_MOTOR_1_FORWARD, BACK_MOTOR_1_BACKWARD,
	BACK_MOTOR_2_FORWARD, BACK_MOTOR_2_BACKWARD
};
uint8_t pwm_channels[] = {
	CH_FRONT_1A, CH_FRONT_1B,
	CH_FRONT_2A, CH_FRONT_2B,
	CH_BACK_1A, CH_BACK_1B,
	CH_BACK_2A, CH_BACK_2B
};

// --------------------------------------------------------------------------
// MOTORS SETUP IMPLEMENTATION
// --------------------------------------------------------------------------

void motors_setup() {
	for (uint8_t i = 0; i < sizeof(pwm_channels); i++) {
		// Initializing 8 PWM channels with wanted frequency and resolution
		ledcSetup(pwm_channels[i], PWM_FREQUENCY, PWM_RESOLUTION);

		// Binding the motor driver pins to their corresponding channels
		ledcAttachPin(motor_pins[i], pwm_channels[i]);
	}
}

// --------------------------------------------------------------------------
// MOTORS FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

int8_t set_motors(char direction, int16_t throttle) {
	// Power motors based on direction and throttle (0 - 1023)
	for (uint8_t i = 0; i < sizeof(pwm_channels); i++) {
		switch (direction) {
			case 'F':
				ledcWrite(pwm_channels[i], (0 == i % 2) ? throttle : 0);
				break;
			case 'B':
				ledcWrite(pwm_channels[i], (0 != i % 2) ? throttle : 0);
				break;
			case 'S':
				ledcWrite(pwm_channels[i], 1023);
				break;
			default:
				return -1;
		}
	}
	return 0;
}

void test_motors(int16_t throttle, int32_t delay_ms) {
	const char* motors_name[] = {
		"FRONT_MOTOR_1_FORWARD", "FRONT_MOTOR_1_BACKWARD",
		"FRONT_MOTOR_2_FORWARD", "FRONT_MOTOR_2_BACKWARD",
		"BACK_MOTOR_1_FORWARD", "BACK_MOTOR_1_BACKWARD",
		"BACK_MOTOR_2_FORWARD", "BACK_MOTOR_2_BACKWARD",
	};

	// Testing each motor based on throttle (0 - 1023)
	for (uint8_t i = 0; i < sizeof(pwm_channels); i++) {
		WebSerial.printf("Testing motor: %s | Throttle: %u\n", motors_name[i],
						  throttle);
		for (uint8_t j = 0; j < sizeof(pwm_channels); j++) {
			ledcWrite(pwm_channels[j], (i == j) ? throttle : 0);
		}
		if (0 < delay_ms) {
			delay(delay_ms);
		}
	}

	// Stopping all motors
	WebSerial.printf("Motors test finished. Check for faulty motors.\n");
	for (uint8_t i = 0; i < sizeof(pwm_channels); i++) {
		ledcWrite(pwm_channels[i], 0);
	}
}