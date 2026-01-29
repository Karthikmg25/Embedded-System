/*
 * lcd.h
 *
 * Created: 19-12-2025 16:58:06
 *  Author: hp
 */ 


#ifndef LCD_H_
#define LCD_H_


#include <avr/io.h>
#include <util/delay.h>
#define EN PORTB3
#define RS PORTB4

void enable_pulse();
void send_nibble(uint8_t nib);
void lcd_cmnd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_init();
void lcd_string(char *str);
void lcd_num(uint16_t num);
void lcd_goto(uint8_t row, uint8_t col);
void lcd_float(float value);



#endif /* LCD_H_ */