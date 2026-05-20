/*
 * BMP280_OLED_I2C.c
 *
 * Created: 18-05-2026 18:46:02
 * Author : hp
 */ 
 /*   Interfacing BMP280 & OLED Using I2C protocol   */
 /****************************************************/

 #define F_CPU 16000000UL
 #include <avr/io.h>
 #include <util/delay.h>
 #include "I2C.h"
 #include "UART.h"
 #include "OLED.h"
 #include "BMP280_I2C.h"


 int main(void)
 {
	 // initialize I2C
	 Init_I2C();
	 
	 // initialize UART
	 Init_UART();
	 
	 // initialize and clear OLED display
	 OLED_init();
	 OLED_clear_display();
	 
	 // Reset OLED cursor to top left
	 OLED_reset_cursor();
	 
	 
	 // Add a small delay before communication
	 _delay_ms(100);
	 
	 // Steps for reading BMP280 sensor:
	 
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
	 
	 // Print sensor ID on OLED display
	 // string name, page, column
	 OLED_print_string("Sensor ID:",2,0);
	 OLED_print_HEXnum(sensor_id,2, 80);
	
	 OLED_print_string("Temperature:",4, 0);
	
	 while (1)
	 {
		 
		 // Step 4: Read ADC values in loop
		 // - combine MSB,LSB,XLSB to form 20-bit ADC value
		 uint8_t ADC_data[3];
		 BMP_Read_Multi_Byte(0xFA, ADC_data, 3);
		 uint32_t ADC_value = (((uint32_t)ADC_data[0]<< 12)| ((uint32_t)ADC_data[1]<< 4) |((uint32_t)ADC_data[2]>> 4));
		 
		 // Step 5: Calculate Temperature using ADC and calibration values
		 int32_t var1 = (((((int32_t)ADC_value>> 3) - ((int32_t)dig_t1 << 1))) * ((int32_t)dig_t2)) >> 11;
		 int32_t var2 = ((((((int32_t)ADC_value >> 4) - (int32_t)dig_t1) * (((int32_t)ADC_value >> 4) - (int32_t)dig_t1)) >> 12) *(int32_t)dig_t3) >> 14;
		 int32_t t_fine = var1 + var2;
		 int32_t T = (t_fine * 5 + 128) >> 8;
		 
		 
		 // Print temperature on UART terminal: Use Fixed points
		 UART_Send_string("\n\rTemperature : ");
		 UART_Send_32bitsigned(T/100);
		 UART_Send_data('.');
		 UART_Send_32bitsigned(T%100);
		 
		 // Print temperature on OLED display: Use Fixed points
		 OLED_print_signed_num(T/100, 4, 92);
		 OLED_print_char('.', 4,104);
		 OLED_print_signed_num(T%100, 4, 110);
		
		 _delay_ms(500);
		 
	 }
 }

