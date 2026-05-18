/*
 * LCD.c
 *
 * Created: 17-05-2026 18:54:20
 *  Author: hp
 */ 

// Define ENABLE and RS pins
#define EN_PIN PORTB3
#define RS_PIN PORTB4

#include "LCD.h"

void enable_pulse()
{
	// Send enable pulse
	PORTB|=(1<<EN_PIN);
	_delay_us(1);
	PORTB&=~(1<<EN_PIN);
	_delay_us(50);
}
void send_nibble(uint8_t nib)
{
	// RS=0  -> sending command
	PORTB&=~(1<<RS_PIN);
	
	PORTD=PORTD& 0x0F;// clear pins PD7-PD4
	PORTD|=(nib<<4);// send nibble
	
	enable_pulse();
	// provide delay after sending commands & data
	_delay_ms(2);
	
}
void lcd_cmnd(uint8_t cmd)
{
	// RS=0 : command
	PORTB&=~(1<<RS_PIN);
	
	// send upper nibble of command
	PORTD=(PORTD&0x0F)|(cmd&0xF0);
	
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(cmd<<4);// send lower nibble of command
	
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_data(uint8_t data)
{
	PORTB|=(1<<RS_PIN);// RS=1 : data
	
	PORTD=(PORTD&0x0F)|(data&0xF0);// send upper nibble of data
	
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(data<<4);// send lower nibble of data
	
	enable_pulse();
	
	_delay_ms(2);
}
void Init_lcd()
{
	// time to stabilize
	_delay_ms(20);
	
	// set ENABLE Pin as low
	PORTB&=~(1<<EN_PIN);
	
	// set 8 bit mode
	send_nibble(0x03);
	_delay_ms(5);
	send_nibble(0x03);
	_delay_us(150);
	send_nibble(0x03);
	_delay_us(150);
	
	// set 4 bit mode
	send_nibble(0x02);
	
	// function set : a-bit mode, 2 line, 5*8 font
	lcd_cmnd(0x28);
	
	// display ON
	lcd_cmnd(0x0c);
	
	// entry mode : Increment cursor
	lcd_cmnd(0x06);
	
	// clear display
	lcd_cmnd(0x01);
	
	_delay_ms(2);
	
}
void lcd_send_string(char *str)
{
	while(*str)
	{
		lcd_data(*str);
		str++;
	}
}
void lcd_set_cursor(uint8_t row, uint8_t col)
{
	// set cursor to given position (row,col)
	
	uint8_t pos;
	
	if(row==0)
	{
		pos= 0x80+ (col&0x0F);
	}
	else
	{
		pos= 0xC0+ (col&0x0F);
	}
	// go to that position
	lcd_cmnd(pos);
}
void lcd_send_number(unsigned int num)
{
	char string[10];
	// convert number to a string
	// use format specifier %09u to pad with 9 zeros
	sprintf(string, "%09u", num);
	
	lcd_send_string(string);
}
void lcd_clear_display()
{
	// clear display
	lcd_cmnd(0x01);
	
	_delay_ms(2);
}
void lcd_reset()
{
	// Display zero
	lcd_send_number(0);
}
void Init_pins()
{
	// PB3, PB4 as output
	DDRB |= (1<<EN_PIN) | (1<<RS_PIN);
	// PD7-PD4 as output
	DDRD |= 0xF0;
	// PD2 as input for push button
	DDRD &=~(1<< DDD2);
	// enable pull up resistor
	PORTD |= (1<< PORTD2);
}