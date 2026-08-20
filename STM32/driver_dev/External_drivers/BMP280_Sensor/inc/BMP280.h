/*
 * BMP280.h
 *
 *  Created on: Aug 15, 2026
 *      Author: hp
 */

#ifndef BMP280_SENSOR_INC_BMP280_H_
#define BMP280_SENSOR_INC_BMP280_H_

//required header files
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_spi_driver.h"

// Define interface configuration for BMP280 communication
/*
 * Usecase:
 * Stores the SPI peripheral and slave-select GPIO configuration
 * required by the BMP280 driver, allowing the sensor to use any
 * SPI instance and SS pin without hardcoding hardware details.
 */
typedef struct
{
	SPI_Reg_t *spi;// SPI peripheral used for BMP280 communication
	GPIO_Reg_t *ss_port;// GPIO port used for chip select
	uint8_t     ss_pin;// GPIO pin used for chip select

}BMP280_Interface_t;

//define structure for BMP280 configuration details
typedef struct
{
	uint8_t BMP_TemperatureOversampling;
	uint8_t BMP_PressureOversampling;
	uint8_t BMP_MeasurementMode;
}BMP280_Configurables_t;

//define structure for BMP280 calibration values
typedef struct
{
	//temperature
	uint16_t dig_t1;
	int16_t  dig_t2;
	int16_t  dig_t3;

	//pressure
	uint16_t dig_P1;
	int16_t  dig_P2;
	int16_t  dig_P3;
	int16_t  dig_P4;
	int16_t  dig_P5;
	int16_t  dig_P6;
	int16_t  dig_P7;
	int16_t  dig_P8;
	int16_t  dig_P9;

	//


}BMP280_CalibValues_t;

//define structure for BMP280 Sensor details
typedef struct
{
	BMP280_Interface_t interface;
	uint8_t ID;
	BMP280_Configurables_t configurations;
	BMP280_CalibValues_t calib_values;
	int32_t t_fine;
	float temperature;
	float pressure;

}BMP280_t;

//define configuration macros:
typedef enum
{
	BMP280_OVERSAMPLING_SKIP = 0,
	BMP280_OVERSAMPLING_X01  = 1,
	BMP280_OVERSAMPLING_X02  = 2,
	BMP280_OVERSAMPLING_X04  = 3,
	BMP280_OVERSAMPLING_X08  = 4,
	BMP280_OVERSAMPLING_X16  = 5,

}BMP280_Oversampling_t;

typedef enum
{
	BMP280_MODE_SLEEP  = 0x00,
	BMP280_MODE_FORCED = 0x01,
	BMP280_MODE_NORMAL = 0x03

}BMP280_Mode_t;

//define BMP register addresses

#define BMP280_REG_ID                0xD0
#define BMP280_REG_CTRL_MEAS         0xF4
/* Pressure data*/
#define BMP280_REG_PRESSURE_MSB      0xF7
#define BMP280_REG_PRESSURE_LSB      0xF8
#define BMP280_REG_PRESSURE_XLSB     0xF9
/* Temperature data*/
#define BMP280_REG_TEMP_MSB          0xFA
#define BMP280_REG_TEMP_LSB          0xFB
#define BMP280_REG_TEMP_XLSB         0xFC
/* Temperature calibration register*/
#define BMP280_REG_TEMP_CALIB        0x88

/* Pressure calibration register */
#define BMP280_REG_PRESSURE_CALIB    0x8E

/* Read-Write Masks*/
#define BMP280_SPI_READ     0x80
#define BMP280_SPI_WRITE    0x7F

/*
 *
 * define Higher level APIs
 *
 */
void BMP280_Init(BMP280_t *bmp);

void BMP280_ReadTemperature(BMP280_t *bmp);

void BMP280_ReadPressure(BMP280_t *bmp);

//API for user to implement custom configurations
void BMP280_Configure(BMP280_t *bmp);

#endif /* BMP280_SENSOR_INC_BMP280_H_ */
