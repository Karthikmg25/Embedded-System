# BMP280 SPI Driver for STM32F401RE


## Overview

This driver provides a lightweight, hardware-abstracted interface for reading temperature and pressure from the Bosch BMP280 sensor over SPI. It is designed for STM32F401RE bare-metal projects using any SPI peripheral and any GPIO line as chip select (CS). The driver handles chip identification, calibration coefficient loading, digital compensation, and configurable oversampling/mode settings.

## Features

Chip ID verification – reads and stores the device ID from register 0xD0.
Automatic calibration – loads all temperature and pressure compensation coefficients during initialization.
Compensated readings – returns temperature in °C and pressure in Pa using Bosch’s recommended algorithms.
Flexible configuration – choose from oversampling options (skip, ×1, ×2, ×4, ×8, ×16) and operation modes (sleep, forced, normal).
Interface agnostic – works with any SPI instance and SS pin by passing pointer to SPI registers and GPIO port/pin.
Simple API – three main functions for init, temperature, and pressure reading.

Language: C (C99)
Target: STM32F401RE
Dependencies: Custom SPI driver (stm32f401re_spi_driver.h), custom GPIO driver (stm32f401re_gpio_driver.h)
Sensor: Bosch BMP280


## Setup

* Add BMP280.c and BMP280.h to your project.
* Ensure you have implementations for the following functions from your SPI driver:
	SPI_Transmit_Byte(SPI_Reg_t*, uint8_t)
	SPI_Receive_Byte(SPI_Reg_t*)
	SPI_Receive_Buffer(SPI_Reg_t*, uint8_t*, uint8_t)
* Initialize your SPI peripheral and the GPIO pin used as chip select before calling the driver.
* Include BMP280.h in your application code.

Note: The driver does not initialize SPI or GPIO. You must configure them separately per your project’s requirements.

## Usage

Minimal Flow: 

* Create a BMP280_t structure.
* Assign the SPI instance and SS pin to bmp.interface.
* Call BMP280_Init().
* Call BMP280_ReadTemperature().
* Call BMP280_ReadPressure() (must follow temperature reading).

## code:

```
#include "BMP280.h"

int main(){

// First configure spi1 and GPIOs

BMP280_t bmp;

bmp.interface.spi      = SPI1;
bmp.interface.ss_port  = GPIOA;
bmp.interface.ss_pin   = 4;   // CS connected to PA4

BMP280_Init(&bmp);

	while(1){

	// Read temperature (must be called first to compute t_fine)
	BMP280_ReadTemperature(&bmp);
	float temp = bmp.temperature; // °C

	// Read pressure
	BMP280_ReadPressure(&bmp);
    float pressure = bmp.pressure; // Pa
  }
}
```
### Configuration Example

Before calling BMP280_Init() or after, you can override the default settings. To apply custom settings, modify the BMP280_Configurables_t fields and call BMP280_Configure().

c

// Example: ×4 temperature oversampling, ×2 pressure oversampling, forced mode
bmp.configurations.BMP_TemperatureOversampling = BMP280_OVERSAMPLING_X04;
bmp.configurations.BMP_PressureOversampling    = BMP280_OVERSAMPLING_X02;
bmp.configurations.BMP_MeasurementMode         = BMP280_MODE_FORCED;

BMP280_Configure(&bmp);  // optional, if you don't want defaults


### Default settings (applied by BMP280_Init):

Temperature oversampling: ×1
Pressure oversampling: ×1
Mode: normal


## API Documentation:

* void BMP280_Init(BMP280_t *bmp)
	Reads chip ID, loads calibration coefficients, and applies default configuration to ctrl_meas register.

* void BMP280_Configure(BMP280_t *bmp)
	Writes the user-defined configuration to the ctrl_meas register. Call after updating bmp.configurations.

* void BMP280_ReadTemperature(BMP280_t *bmp)
	Reads raw ADC temperature, computes compensated value, stores it in bmp.temperature (float, °C). Also stores bmp.t_fine for pressure compensation.

* void BMP280_ReadPressure(BMP280_t *bmp)
	Reads raw ADC pressure, computes compensated value, stores it in bmp.pressure (float, Pa).

Important: BMP280_ReadTemperature() must be called before BMP280_ReadPressure() because pressure compensation requires t_fine.

## Testing

No formal test suite is included. Basic verification steps:

* Check bmp.ID after init: should read 0x58.
* Compare temperature reading with a known reference sensor.
* Check pressure reading within expected atmospheric range (± few hPa).
  Deployment
This driver is intended for bare-metal embedded use. It compiles with standard C89/C99 and has no OS dependencies.
