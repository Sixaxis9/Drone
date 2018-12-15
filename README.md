# Paperino - An open source drone

In this repository you will find the final version of a complete solution to stabilize and drive a quadcopter. The firmware is coded in C within Ateml Studio IDE.
Inside the "Solution" folder you'll find the project ready to be opened by Atmel Studio, while in the "Code" folder contains only the source files.

## Considerations on data filtering and sensor fusion

Below can be found some graph displaying raw measurements, pitch and roll converted values and filtered ones. The graph relates to laboratory taken measures, and are thought to simulate worst case scenarios from real flight condition.

## Getting started
 
In this section you'll find how to get started with Paperino's firmware.

### Opening solution in Atmel Studio

To open the project, start Atmel Studio, click on "Open Project" and select the file with .prj extension.

### Build and load

To upload and build the files you have to plug in your AVR-ISP programmer, and select it from the "Tools" menù. please verify the chip signature, in my case "", the one for Atmega32u4.

### Test, fly, enjoy!

It's now time to test your setup and enjoy flying around. USART debug is set by default, so you can attach a TTL serial adapter to RX-TX pins and read data. 57.6kBaud, 1 bit stop, 0 parity bits.

## In depth anaysis of the firmware

All the firmware is quite well- commented, but for additional clarity I'll write something on its core functions.

### TWI interface

I've selected the MPU6050 accelerometer+gyroscope becasue it is cheap, quite precise and can be found pretty easly. The communication relies on a i2c bus, handled by hardware from the Atmega. All the registers are set in the library `i2c.c` and can be accessed by the functions: 
```
void TWI_transmit_8bit();
```
The transmit functions 

At startup the IMU is initialize writing to proper registers
