# Classic Car Logger & Display Console
This project intention is to interface with classic carb fed engines, recording and displaying the live data

## Current Project Scope
### Engine interface
Power by a STM32F103C8 (mostly due to its advantage that sweet 64K ROM and 3x hardware UART )
Interfacing with:
- NTC based coolant sensor - Bosch 0 280 130 026 (very common equiviliant in most engines)
- Oil Pressure 
- Tachometer, interfacing with the coil pickup
- GPS, for speed and location
- Gyro, for acceleration, roll, pitch, yaw
- Peddle possions
- SD card logging

### Console Display
Power by ATmega 328 , it will be driving 4x 1602 HD444780 displays
The LCD will be interfaced via SPI, using 74hc595 to reduce the IO requirements
This console is to be used as a gauge cluster replacement, for racing or otherwise.
Large font is used for easy to read status
The Engine Interface MCU will send required data to the console
