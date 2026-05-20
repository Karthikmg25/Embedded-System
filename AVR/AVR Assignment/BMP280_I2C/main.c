/*
 * BMP280_I2C.c
 *
 * Created: 20-05-2026 13:53:23
 * Author : hp
 *
 */
 /*   Interfacing BMP280 Sensor Using I2C protocol   */
 /****************************************************/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#include "UART.h"
#include "BMP280_I2C.h"


int main(void)
{
	Init_I2C();
	Init_UART();
	
	
	// Add a small delay before communication
	_delay_ms(100);
	
		
		/**********************************************/
		/*    Steps of BMP280 Temperature Reading     */
		/**********************************************/
		
		// 1) Read Sensor ID verification
		// 2) Read 3 calibration values for temperature calculation
		//    - These are 16 bit values (dig_t1,dig_t2,dig_t3)
		//    - Stored in little endian format at  register address: 0x88-0x8D (6 bytes)
		//    - digt2 and digt3 are signed values
		// 3) Configure control meas register
		//    - Register address              : 0xF4
		//    - Set temperature oversampling -> 0x1
		//    - Set pressure oversampling    -> 0x1
		//    - Set power mode               -> normal
		//    - These are set by writing     -> 0x27
		// 4) Read 20-bit raw ADC value at register address: 0xFA-0xFC
		//    - 0xFA - MSB
		//    - 0xFB - LSB
		//    - 0xFC - XLSB (last 4 bits-> stored at XLSB 7:4)
		//    - Combine this to form 20 bit ADC value
		// 5) Calculate Temperature using ADC and calibration values
		
	// Step 1: reading sensor ID
	uint8_t sensor_id= BMP_Read_Byte(0xD0);
	
	// Step 2: reading calibration values
	// - Use 6 bytes to form three 16-bit values
	uint8_t calib_data[6];
	BMP_Read_Multi_Byte(0x88, calib_data, 6);
	uint16_t dig_t1= (((uint16_t)calib_data[1]<<8)|((uint16_t)calib_data[0]));
	int16_t dig_t2 = (((int16_t)calib_data[3]<<8) | ((int16_t)calib_data[2]));
	int16_t dig_t3 = (((int16_t)calib_data[5]<<8) | ((int16_t)calib_data[4]));
	
	// Step 3: Configure control meas register
	BMP_Write_Byte(0xF4, 0x27);
	
	
	 // Print sensor ID and and calibration values:
	 
	 UART_Send_string("\n\rSensor ID of BMP280 : ");
	 // Print Hexadecimal format
	 UART_Send_HEXnumber(sensor_id);
	 
	 UART_Send_string("\n\rCalibration values of BMP280 \n");
	 UART_Send_string("\n\rdig_t1 : ");
	 UART_Send_number(dig_t1);
	 UART_Send_string("\n\rdig_t2 : ");
	 UART_Send_signed_number(dig_t2);
	 UART_Send_string("\n\rdig_t3 : ");
	 UART_Send_signed_number(dig_t3);
	 
	 
	
    while (1) 
    {
		 // Step 4: Read ADC values in loop
		 // - combine MSB,LSB,XLSB to form 20-bit ADC value
		 uint8_t ADC_data[3];
		 BMP_Read_Multi_Byte(0xFA, ADC_data, 3);
		 uint32_t ADC_value = (((uint32_t)ADC_data[0]<< 12)| ((uint32_t)ADC_data[1]<< 4) |((uint32_t)ADC_data[2]>> 4));
		 
		 UART_Send_string("\n\rraw ADC value : ");
		 UART_Send_32bitnumber(ADC_value);
		 
		 // Step 5: Calculate Temperature using ADC and calibration values
		 int32_t var1 = (((((int32_t)ADC_value>> 3) - ((int32_t)dig_t1 << 1))) * ((int32_t)dig_t2)) >> 11;
		 int32_t var2 = ((((((int32_t)ADC_value >> 4) - (int32_t)dig_t1) * (((int32_t)ADC_value >> 4) - (int32_t)dig_t1)) >> 12) *(int32_t)dig_t3) >> 14;
		 int32_t t_fine = var1 + var2;
		 int32_t T = (t_fine * 5 + 128) >> 8;
		 
		 UART_Send_string("\n\rvar1        : ");
		 UART_Send_32bitsigned(var1);
		 UART_Send_string("\n\rvar2        : ");
		 UART_Send_32bitsigned(var2);
		 UART_Send_string("\n\rt fine      : ");
		 UART_Send_32bitsigned(t_fine);
		 // Use fixed points to print decimal values
		 UART_Send_string("\n\rTemperature : ");
		 UART_Send_32bitsigned(T/100);
		 UART_Send_data('.');
		 UART_Send_32bitsigned(T%100);
		 
			
		_delay_ms(1000);
		
    }
}

