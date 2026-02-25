/*
 * OLED.h
 *
 * Created: 19-01-2026 16:01:24
 *  Author: hp
 */ 


#ifndef OLED_H_
#define OLED_H_
#include <avr/io.h>
#define F_CPU 16000000UL
void OLED_init();
void OLED_print_char(char ch, uint8_t page, uint8_t col);
void OLED_print_string(char *str,uint8_t page, uint8_t col);
void OLED_print_num(uint16_t num,uint16_t page,uint16_t col);
void OLED_clear_space(uint8_t page, uint8_t col, uint8_t space);
void OLED_clear_display();
void OLED_reset_cursor();
void initialise_OLED_buffer();
#endif /* OLED_H_ */