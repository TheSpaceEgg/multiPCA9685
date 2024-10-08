# multiPCA9685 Library

## Description
The `multiPCA9685` library is designed to manage and control an unlimited number of PCA9685 devices via a single object. This simplifies the control of large numbers of PWM devices (such as servos or LEDs) by abstracting the management of multiple I2C devices into a single controller class.

## Author
- **Will Hickmott**  
  GitHub: [TheSpaceEgg](https://github.com/TheSpaceEgg)

## Features
- Control unlimited numbers of PCA9685 devices across multiple I2C buses.
- Set PWM frequency and individual PWM signals for each device.
- Debug mode to track and monitor the status of each device.
- Easily scalable for complex hardware setups.

## Dependencies
This library depends on the `i2cdevice` library, which handles I2C communication. You can find it here:  
[i2cdevice GitHub Repository](https://github.com/TheSpaceEgg/i2cdevice)

## Usage
To use the `multiPCA9685` library, include the library and its dependencies in your project. Here’s an example of basic usage:

```cpp
#include "multiPCA9685.h"

int main() {
    std::vector<std::pair<int, uint8_t>> devices = {{1, 0x40}, {1, 0x41}, {2, 0x42}};
    MultiPCA9685 PCAobj(devices, 50);

    // Set PWM for the first motor on the first PCA9685
    PCAobj.setPWM(0, 0, 2000);
    // Set PWM for the first motor on the third PCA9685
    PCAobj.setPWM(47, 0, 2000);

    // Output current setup
    PCAobj.getSetup();

    return 0;
}
