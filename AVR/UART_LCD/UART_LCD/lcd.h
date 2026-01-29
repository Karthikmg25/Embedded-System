/*
 * lcd.h
 *
 * Created: 29-12-2025 18:03:10
 *  Author: hp
 */ 


#ifndef LCD_H_
#define LCD_H_

void enable_pulse();
void nibble_send(uint8_t nib);
void lcd_cmd(uint8_t cmd);
void lcd_data(uint8_t data);
void lcd_string(char *str);
void lcd_init();
void lcd_goto(uint8_t row, uint8_t col);
void lcd_num(uint16_t num);
void lcd_float(float value);
#endif /* LCD_H_ */