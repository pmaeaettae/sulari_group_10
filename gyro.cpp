#include "gyro.h"
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void mpu_init() {
    Wire.begin(2, 1); // SDA, SCL connections to pins on the ESP32
    Wire.beginTransmission(0x68); // Address is 0x68
    mpu.initialize();
}

// This function checks if the player is stationary
bool mpu_stationary() {
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    
    static long previous = 0;
    long mag = abs(ax) + abs(ay) + abs(az);
    long diff = abs(mag - previous);
    previous = mag;

    return diff < 2000; // Note: fine tuning for this probably required
}