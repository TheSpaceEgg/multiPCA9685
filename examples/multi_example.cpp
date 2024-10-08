/**
 * @file multi_example.h
 * @author Will Hickmott
 * @github TheSpaceEgg
 * @brief This file is part of the multiple instances PCA9685 library.
 * 
 * This code is provided under total free-use rights.
 * You are free to use, modify, and distribute this code without restriction.
 */
 
 #include "PCA9685.h"
#include <iostream>

// get these values experimentally or from datasheet for your servos
#define SERVOMIN    110
#define SERVOMAX    550
 
    // multiPCA constructor expects a vector of pairs of i2c bus numbers and 
    std::vector<std::pair<int, uint8_t>> pcaBusAddressPairs = {{1, 0x40}, {3, 0x41}, {1, 0x42}, {1, 0x48}, {4, 0x55}, {4, 0x80}}; // Add more bus/address pairs as needed
    
    // Instantiates object of multiple PCA devices with default dev frequency of 50Hz
    MultiPCA9685 manyPCA9685s(pcaBusAddressPairs);
    
    // Instantiate with different dev frequency
    // MultiPCA9685 manyPCA9685s(pcaBusAddressPairs, 60);

int main() {

    // Check config of your setup
    manyPCA9685s.getSetup();
    /*
    This example should output (providing i2c buses are configured):
        MultiPCA9685 Configuration: 
        Number of PCA9685 Drivers: 6
        Device Frequency: 50 Hz

        Driver 1 -> I2C Bus: 1, Address: 0x40
        Driver 2 -> I2C Bus: 3, Address: 0x41
        Driver 3 -> I2C Bus: 1, Address: 0x42
        Driver 4 -> I2C Bus: 1, Address: 0x48
        Driver 5 -> I2C Bus: 4, Address: 0x55
        Driver 6 -> I2C Bus: 4, Address: 0x80
    */

    manyPCA9685s.setPWM(17, SERVOMIN);
    usleep(1000000);
    manyPCA9685s.toggleDebug(); // Turns on debug output
    manyPCA9685s.setPWM(17, SERVOMAX);
    usleep(1000000);

    manyPCA9685s.setPWM(21, SERVOMIN);
    usleep(1000000);
    manyPCA9685s.setPWM(21, SERVOMAX);
    usleep(1000000);
    manyPCA9685s.toggleDebug(); // Turns off debug output

    manyPCA9685s.setPWM(95, SERVOMIN);
    usleep(1000000);
    manyPCA9685s.setPWM(95, SERVOMAX);
    usleep(1000000);

    return 0;
}
