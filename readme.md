# Multiple VL53L0X Sensor Data Reading System

Video:
https://youtu.be/OrfpyjqpGTM

This project utilizes an Arduino and a TCA9548 I2C multiplexer to read data from multiple VL53L0X laser distance sensors.
This is a low-cost obstacle avoidance solution, with an estimated cost under 10 USD.
VL53L0x approximately 0.9 USD
TCA9548 approximately 0.2 USD
F103c8t6 approximately 1 USD

## Features

- Manage up to 8 VL53L0X sensors simultaneously through the TCA9548 I2C multiplexer.
- Read distance data from each sensor and calculate the average to reduce random errors.
- Transmit average distance data through serial port in a specific format.

## Hardware Requirements

- Arduino or compatible board
- TCA9548 I2C multiplexer
- Up to 8 VL53L0X laser distance sensors
- Appropriate connecting wires

## Wiring Guide

Connect TCA9548's SDA and SCL wires to the Arduino's SDA_PIN and SCL_PIN, and connect each VL53L0X sensor to the respective channels of TCA9548.

## Instructions

1. Upload the code to your Arduino.
2. Open the serial monitor to view data from the sensors.
3. Observe the serial port output to ensure the sensor data is being transmitted correctly.

## Code Description

The code includes the following sections:

- Initialize and set the serial communication rate.
- Initialize the TCA9548 I2C multiplexer.
- Loop to read data from each VL53L0X sensor on every channel and store.
- Calculate the average distance for each sensor and transmit through the serial port.

## Python Tools

Install dependencies

```
pip install -r requirements.txt
```

Run

```
python gui.py
```

## Note

- Ensure all sensors are correctly connected and there are no address conflicts.
- You may need to adjust the I2C addresses and pin configurations according to your hardware setup.

## Contribution

If you have any suggestions or improvements, feel free to submit a pull request or open an issue.
