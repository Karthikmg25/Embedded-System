/*
 * LCD_interfacing.c
 *
 * Created: 18-12-2025 20:24:13
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"

int main(void)
{
	DDRD=0xF0;// set PD7-PD4 as output
	DDRB|=(1<<RS)|(1<<EN);// set RS and EN pin as output
    lcd_init();
	
	lcd_string("AVR LCD");
	
	lcd_goto(1,0);// go to 2nd line
	
	lcd_string("Learning");
	
    while (1) 
    {
		
		
		
    }
}

