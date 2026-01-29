/*
 * lcd.c
 *
 * Created: 18-12-2025 20:24:39
 *  Author: hp
 */ 

#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

 void enable_pulse()
{
	EN_PORT|=(1<<EN);
	
	_delay_us(1);
	
	EN_PORT&=~(1<<EN); // sending enable pulse
	_delay_us(50);
}

 void lcd_nibble(uint8_t nib)
{
	RS_PORT&=~(1<<RS);// RS=0 ; command mode
	
	PORTD=(PORTD & 0x0F) | (nib<<4);// send nibble to PD7-PD4
	
	enable_pulse();
	
	_delay_ms(2);// LCD needs time for execution
}

void lcd_cmnd(uint8_t cmd)
{
	RS_PORT&=~(1<<RS);// RS =0 : command mode
	
	PORTD=(PORTD & 0x0F) | (cmd & 0xF0);// send upper nibble of command
	
	enable_pulse();
	
	PORTD=(PORTD & 0x0F) | (cmd<<4);// send upper nibble of command
	
	enable_pulse();
	
	_delay_ms(2);// wait after command
}
void lcd_data(uint8_t data)
{
	RS_PORT|=(1<<RS);// RS=1 : data mode
	
	PORTD=(PORTD & 0x0F) | (data&0XF0);// send upper nibble of data
	
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(data<<4);// send lower nibble of data
	
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_clear()
{
	lcd_cmnd(0x01);
	
	_delay_ms(2);
}
void lcd_home()
{
	lcd_cmnd(0x02);
	
	_delay_ms(2);
}
void lcd_init()
{
	_delay_ms(20);// wait for LCD stabilization
	
	EN_PORT&=(1<<EN);// set EN low for safety
	
	
	lcd_nibble(0x03);// force 8 bit mode initially
	_delay_ms(5);
	
	lcd_nibble(0x03);// LCD will be in an unknown state at the start
	_delay_us(150);
	
	lcd_nibble(0x03);
	_delay_us(150);
	
	
	lcd_nibble(0x02);// force 4 bit mode
	
	lcd_cmnd(0x28);// function set - 4 bit mode, 2 line display, 5*7 pixel
	
	lcd_cmnd(0x0C);// display ON, cursor OFF, blink OFF
	
	lcd_cmnd(0x06);// entry mode- increment cursor
	
	lcd_cmnd(0x01);// clear display
	
	_delay_ms(2);// wait for clearing
	
}
void lcd_goto(uint8_t row, uint8_t col)
{
	uint8_t pos;
	
	if(row==0)
	{
		pos= 0x80 + (col& 0x0F);// starting address of first line= 0x80
	}
	else
	{
		pos= 0xC0 + (col& 0x0F);// starting address of second line= 0xC0
	}
	
	lcd_cmnd(pos);// go to that position
	
}
void lcd_string(char *str)
{
	while(*str)
	{
		lcd_data(*str);
		str++;
	}
}
void lcd_num(uint16_t num)
{
	char str[6];// string to allocate 5 digits + '\0'
	
	itoa(num,str,10);// convert num to a string
	
	lcd_string(str);
}