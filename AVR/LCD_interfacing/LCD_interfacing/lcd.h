/*
 * lcd.h
 *
 * Created: 18-12-2025 20:25:08
 *  Author: hp
 */ 


#ifndef LCD_H_
#define LCD_H_
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>

#define RS_PORT PORTB
#define RS PORTB4
#define EN_PORT PORTB   // macros
#define EN PORTB3

 void enable_pulse();
 void lcd_nibble(uint8_t nib);
void lcd_init();
void lcd_cmnd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_string(char *str);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_num(uint16_t num);
void lcd_float(float value);  // function declarations
void lcd_clear();
void lcd_home();




#endif /* LCD_H_ */