/*
 * UART_LCD.c
 *
 * Created: 29-12-2025 17:56:25
 * Author : hp
 */ 
#define F_CPU 16000000UL
#define RS PORTB4
#define EN PORTB3 
#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include "uart.h"

int main(void)
{
   lcd_init();
   
   uart_init();
   
   char str[35];
   
    while (1) 
    {
		lcd_goto(0,0);
		
		RX_string(str,sizeof(str));
		
		lcd_cmd(0x01);//clear display
		_delay_ms(2);
		
		lcd_string(str);
		
		
		
    }
}

