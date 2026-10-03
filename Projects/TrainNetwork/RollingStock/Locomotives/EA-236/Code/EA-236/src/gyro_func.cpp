#include "gyro_func.h"
#include "config.h"

// --------------------------------------------------------------------------
// GYROSCOPE SETUP IMPLEMENTATION
// --------------------------------------------------------------------------

bool gyro_setup() {
    // Remapping the I2C pins
    Wire.begin(MPU_SDA, MPU_SCL);
    Wire.beginTransmission(MPU_ADDRESS);
    
    // PWR_MGMT_1 register
    Wire.write(0x6B);
    
    // Writing 0 on the power register (0x6B) for waking the MPU
    Wire.write(0x00);
    return (0 == Wire.endTransmission());
}

// --------------------------------------------------------------------------
// GYROSCOPE FUNCTIONS IMPLEMENTATION
// --------------------------------------------------------------------------

void read_gyro(float *gyro_out) {
    // Reading the axes values from the gyroscope 
    Wire.beginTransmission(MPU_ADDRESS);

    // GYRO_XOUT_H register (starting point)
    Wire.write(0x43);
    Wire.endTransmission(false);
    
    // Asking for 6 bytes (2 for each axis: X, Y, Z)
    Wire.requestFrom((uint8_t)MPU_ADDRESS, (uint8_t)6, (uint8_t)true);

    if (Wire.available() >= 6) {
        // Reading and combining high byte with low byte
        int16_t gyro_x = (Wire.read() << 8 | Wire.read());
        int16_t gyro_y = (Wire.read() << 8 | Wire.read());
        int16_t gyro_z = (Wire.read() << 8 | Wire.read());

        // Dividing with the scaling factor (131.0) for getting deg/sec
        gyro_out[0] = gyro_x / 131.0;
        gyro_out[1] = gyro_y / 131.0;
        gyro_out[2] = gyro_z / 131.0;
    } else {
        gyro_out[0] = 0;
        gyro_out[1] = 0;
        gyro_out[2] = 0;
    }
}