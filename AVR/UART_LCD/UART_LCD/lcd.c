/*
 * lcd.c
 *
 * Created: 29-12-2025 18:02:38
 *  Author: hp
 */ 
#define F_CPU 16000000UL
#define RS PORTB4
#define EN PORTB3
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void enable_pulse()
{
	PORTB|=(1<<EN);// ON
	_delay_us(5);
	PORTB&=~(1<<EN);//OFF
	_delay_us(50);
	
}
void nibble_send(uint8_t nib)
{
	PORTB&=~(1<<RS);// RS=0 - command mode
	PORTD=(PORTD&0x0F)|(nib<<4);// send nibble to PD7-PD4
	enable_pulse();
	_delay_ms(2);// delay needed after each commands
	
}
void lcd_cmd(uint8_t cmd)
{
	PORTB&=~(1<<RS);// RS=0 : command mode
	PORTD=(PORTD&0x0F)|(cmd&0xF0);// send upper nibble
	enable_pulse();
	PORTD=(PORTD&0x0F)|(cmd<<4);// send lower nibble
	enable_pulse();
	_delay_ms(2);
}
void lcd_data(uint8_t data)
{
	PORTB|=(1<<RS);// RS=1 : data mode
	PORTD=(PORTD&0x0F)|(data&0xF0);// send upper nibble
	enable_pulse();
	PORTD=(PORTD&0x0F)|(data<<4);// send lower nibble
	enable_pulse();
	_delay_ms(2);
}

void lcd_init()
{
	DDRD=0xF0;// set PD7-PD4 as outputs
	DDRB|=(1<<EN)|(1<<RS);// set EN and RS as outputs
	
	_delay_ms(20);// delay for LCD stabilization
	PORTB&=~(1<<EN);// set EN as 0
	
	nibble_send(0x03);// force 8 bit mode
	_delay_ms(5);
	nibble_send(0x03);// force 8 bit mode
	_delay_us(150);
	nibble_send(0x03);// force 8 bit mode
	_delay_us(150);
	
	nibble_send(0x02);// force 4 bit mode
	lcd_cmd(0x28);// function set- 2 line, 4 bit, 5*7 pixel
	lcd_cmd(0x0C);// display ON, cur sure and Blink OFF
	lcd_cmd(0x06);// entry mode
	lcd_cmd(0x01);// clear display
	_delay_ms(2);// delay for clearing
	
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
		pos=0x80+(col&0x0F);
	}
	else
	{
		pos=0xC0+(col&0x0F);
	}
	lcd_cmd(pos);
}
void lcd_num(uint16_t num)
{
	char str[6];// 5digits + '\0'
	itoa(num,str,10);// convert to string
	lcd_string(str);
}
void lcd_float(float value)
{
	char str[10];
	dtostrf(value,0,2,str);// convert float value to a string
	lcd_string(str);
}