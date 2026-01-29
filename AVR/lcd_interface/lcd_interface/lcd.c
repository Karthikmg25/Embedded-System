/*
 * lcd.c
 *
 * Created: 19-12-2025 16:57:47
 *  Author: hp
 */ 
#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void enable_pulse()
{
	PORTB|=(1<<EN);
	_delay_us(1);
	PORTB&=~(1<<EN);// enable pulse
	_delay_us(50);
}
void send_nibble(uint8_t nib)
{
	PORTB&=~(1<<RS);// RS=0  ; command
	
	PORTD=PORTD& 0x0F;// clear pins PD7-PD4
	PORTD|=(nib<<4);// send nibble
	
	enable_pulse();
	
	_delay_ms(2);// needs time for execution
	
}
void lcd_cmnd(uint8_t cmd)
{
	PORTB&=~(1<<RS);// RS=0 : command
	
	PORTD=(PORTD&0x0F)|(cmd&0xF0);// send upper nibble of command
	
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(cmd<<4);// send lower nibble of command
	
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_data(uint8_t data)
{
	PORTB|=(1<<RS);// RS=1 : data
	
	PORTD=(PORTD&0x0F)|(data&0xF0);// send upper nibble of data
	
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(data<<4);// send lower nibble of data
	
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_init()
{
	_delay_ms(20);// time to stabilize
	
	PORTB&=~(1<<EN);// set EN as low
	
	send_nibble(0x03);// set 8 bit mode
	_delay_ms(5);
	
	send_nibble(0x03);
	_delay_us(150);
	
	send_nibble(0x03);
	_delay_us(150);
	
	send_nibble(0x02);// set 4 bit mode
	
	lcd_cmnd(0x28);// function set
	
	lcd_cmnd(0x0c);// display ON
	
	lcd_cmnd(0x06);// entry mode
	
	lcd_cmnd(0x01);// clear display
	
	_delay_ms(2);
	
}
void lcd_string(char *str)
{
	while(*str)
	{
		lcd_data(*str);
		str++;
	}
}
void lcd_goto(uint8_t row, uint8_t col)
{
	uint8_t pos;
	
	if(row==0)
	{
		pos= 0x80+ (col&0x0F);
	}
	else
	{
		pos= 0xC0+ (col&0x0F);
	}
	
	lcd_cmnd(pos);// go to that position
}
void lcd_num(uint16_t num)
{
	char a[6];
	
	itoa(num,a,10);// convert num to a string
	
	lcd_string(a);
}
void lcd_float(float value)
{
	uint16_t t=(uint16_t)(value*100);// multiply by 100(precisio 2), convert float to uint16
	
	lcd_num(t/100);// number before decimal point
	
	lcd_data('.');
	
	if((t%100)<10) lcd_data('0');// pad zero
	
	lcd_num(t%100);// after decimal point
}