/**
 * @file multiPCA9685.cpp
 * @author Will Hickmott
 * @github TheSpaceEgg
 * @brief This file is part of the multiple instances PCA9685 library.
 * 
 * This code is provided under total free-use rights.
 * You are free to use, modify, and distribute this code without restriction.
 */

#include "multiPCA9685.h"

/**
 * @brief Constructor for single PCA9685.
 * Initialises the I2C device and sets the device frequency.
 * 
 * @param bus The I2C bus number.
 * @param deviceAddress The I2C address of the PCA9685.
 * @param freq The device frequency in Hz.
 */
PCA9685::PCA9685(uint8_t bus, uint8_t deviceAddress, uint8_t freq) {
    i2c = new I2CDevice(bus, deviceAddress); 
    reset();
    setFreq(freq);
}

/**
 * @brief Resets the PCA9685 by writing to the MODE1 register.
 */
void PCA9685::reset() {
    i2c->writeRegister(MODE1, 0x20);
}
/**
 * @brief Sets the device frequency by calculating the prescale value
 * and writing it to the PRE_SCALE register.
 * 
 * @param freq The device frequency in Hz.
 */
void PCA9685::setFreq(uint8_t freq) {
    uint8_t prescale_val = static_cast<uint8_t>((CLOCK_FREQ / (4096 * freq)) - 1 + 0.5);
    i2c->writeRegister(MODE1, 0x10);
    i2c->writeRegister(PRE_SCALE, prescale_val);
    i2c->writeRegister(MODE1, 0xA0);
}
/**
 * @brief Sets the PWM signal for a specific servo with the default "on" time of 0.
 * 
 * @param servo The servo index (0-15).
 * @param value The "off" time value (0-4095).
 */
void PCA9685::setPWM(uint8_t servo, uint16_t value) {
	setPWM(servo, 0, value);
}
/**
 * @brief Sets the PWM signal for a specific servo with custom on and off times.
 * 
 * @param servo The servo index (0-15).
 * @param on_value The "on" time value (0-4095).
 * @param off_value The "off" time value (0-4095).
 */
void PCA9685::setPWM(uint8_t servo, uint16_t on_value, uint16_t off_value) {

    uint8_t data[4] = {
        static_cast<uint8_t>(on_value & 0xFF), 
        static_cast<uint8_t>(on_value >> 8), 
        static_cast<uint8_t>(off_value & 0xFF), 
        static_cast<uint8_t>(off_value >> 8)
    };
    
    i2c->writeRegister(SERVO0 + MULTIPLIER * servo, data, 4);

}

/**
 * @brief MultiPCA9685 constructor to handle multiple PCA9685 drivers.
 * Initializes each PCA9685 driver with the given bus and address pairs.
 * 
 * @param bus_address_pairs Vector containing bus and address pairs.
 * @param frequency The device frequency in Hz for all devices.
 */
MultiPCA9685::MultiPCA9685(const std::vector<std::pair<int, uint8_t>>& bus_address_pairs, uint8_t frequency)
  : busAddressPairs(bus_address_pairs), numDrivers(bus_address_pairs.size()), DevFrequency(frequency) {
    for (const auto& bus_address : bus_address_pairs) {
        int bus = bus_address.first;
        uint8_t address = bus_address.second;
        pwmDrivers.emplace_back(bus, address, frequency);
    }
}

/**
 * @brief Sets the PWM signal for a motor using the default on time of 0.
 * 
 * @param motorIndex The index of the motor across all drivers.
 * @param val The "off" time value (0-4095).
 */
void MultiPCA9685::setPWM(uint8_t motorIndex, uint16_t val) {
  setPWM(motorIndex, 0, val);
}

/**
 * @brief Sets the PWM signal for a motor with custom on and off times, across multiple PCA9685 drivers.
 * If debug mode is enabled, it prints the details of the command.
 * 
 * @param motorIndex The index of the motor across all drivers.
 * @param on The "on" time value (0-4095).
 * @param off The "off" time value (0-4095).
 */
void MultiPCA9685::setPWM(uint8_t motorIndex, uint16_t on, uint16_t off) {
    uint8_t driverIndex = motorIndex / outputsPerDriver;
    uint8_t channel = motorIndex % outputsPerDriver;
    if (driverIndex < numDrivers) {
        auto& driver = pwmDrivers[driverIndex];
        auto& bus_address = busAddressPairs[driverIndex];
        if(PCA9685_DEBUG){
          std::cout << "Setting PWM (" << on << ", " << off << ") for motor " << static_cast<int>(motorIndex)
                    << " on board " << static_cast<int>(driverIndex)
                    << ", channel " << static_cast<int>(channel)
                    << " at address 0x" << std::hex << static_cast<int>(bus_address.second) << " on i2c bus " << bus_address.first << std::dec
                    << std::endl;
        }
        driver.setPWM(channel, on, off); 
    }
}    

/**
 * @brief Outputs the setup configuration for each PCA9685 driver, including bus number and I2C address.
 */
void MultiPCA9685::getSetup(void) {
  std::cout << "\n\t/*** MultiPCA9685 Configuration: ***/\n";
  std::cout << "\tNumber of PCA9685 Drivers: " << static_cast<int>(numDrivers) << "\n";
  std::cout << "\tDevice Frequency: " << static_cast<int>(DevFrequency) << " Hz\n\n";

  for (size_t i = 0; i < busAddressPairs.size(); ++i) {
      int bus = busAddressPairs[i].first;
      uint8_t address = busAddressPairs[i].second;
      std::cout << "\tDriver " << i + 1 << " -> I2C Bus: " << bus 
                << ", Address: 0x" << std::hex << static_cast<int>(address) << std::dec << "\n";
  }
  std::cout << std::endl;
}

/**
 * @brief Toggles the debug mode on or off. When debug mode is on, it outputs information about the PWM settings.
 */
void MultiPCA9685::toggleDebug() {
  PCA9685_DEBUG = !PCA9685_DEBUG;
  if(PCA9685_DEBUG)
    std::cout << "Debug output On." << std::endl;
  else
    std::cout << "Debug output Off." << std::endl;
}





