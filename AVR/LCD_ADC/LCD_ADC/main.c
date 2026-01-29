/*
 * LCD_ADC.c
 *
 * Created: 20-12-2025 17:34:42
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#define  RS PORTB4
#define EN PORTB3
 
volatile uint16_t value;

void enable_pulse()
{
	PORTB|=(1<<EN);
	_delay_us(1);
	PORTB&=~(1<<EN);
	_delay_us(50);
	
}
void send_nibble(uint8_t nib)
{
	PORTB&=~(1<<RS);// RS=0 : command
	
	PORTD=(PORTD&0x0F)|(nib<<4);// send nibble
	
	enable_pulse();
	
	_delay_ms(2);// delay after each command
}
void lcd_cmd(uint8_t cmd)
{
	PORTB&=~(1<<RS);// RS=0 : command
	
	PORTD=(PORTD&0x0F)|(cmd&0xF0);// send upper nibble
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(cmd<<4);// send lower nibble
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_data(uint8_t data)
{
	
	PORTB|=(1<<RS);// RS=1 : data
	
	PORTD=(PORTD&0x0F)|(data&0xF0);// send upper nibble
	enable_pulse();
	
	PORTD=(PORTD&0x0F)|(data<<4);// send lower nibble
	enable_pulse();
	
	_delay_ms(2);
}
void lcd_init()
{
	_delay_ms(20);// time for LCD stabilization
	
	PORTB&=~(1<<EN);// set EN as low
	
	send_nibble(0x03); // force 8 bit mode
	_delay_ms(5);
	send_nibble(0x03); // force 8 bit mode
	_delay_us(150);
	send_nibble(0x03); // force 8 bit mode
	_delay_us(150);
	
	send_nibble(0x02);// 4 bit mode forced
	
	lcd_cmd(0x28);// function set
	
	lcd_cmd(0x0C);//display on
	
	lcd_cmd(0x06);// entry mode
	
	lcd_cmd(0x01);// clear display
	
	_delay_ms(2);// time for clearing
	
}
void lcd_goto(uint8_t row, uint8_t col)
{
	if(row==0)
	  lcd_cmd(0x80+col);
	else
	  lcd_cmd(0xC0+col);  
	  
	  
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
	char str[6];// 5 digits+null character
	
	itoa(num,str,10);// convert num to a string
	
	lcd_string(str);// print string
	
}
void lcd_float(float value)
{
	uint16_t t= (uint16_t)(value*100);// for precision 2, use 100
	
	lcd_num(t/100);// print integer part
	
	lcd_data('.');// print decimal point
	
	if((t%100)<10) lcd_num(0);// pad zero
	 
	lcd_num(t%100);// print fraction part
	
}

void ADC_setup()
{
	ADMUX|=(1<<REFS0);// reference voltage as AVCC: when on simulide, use AREF
	
	ADMUX=(ADMUX&0xF0)| 0;// ADC0
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS1)|(1<<ADPS2)|(1<<ADPS0);// prescalar 128 , ADC frequency 125KHz
	
	ADCSRA|=(1<<ADSC);// start conversion
	
	while((ADCSRA&(1<<ADSC)));// wait until conversion : ADSC becomes 0 after conversion
	
}

int main(void)
{
	DDRD=0xF0;// set PD7-PD4 as outputs
	DDRB|=(1<<DDB4)|(1<<DDB3);// set EN and RS as outputs
	ADC_setup();
    
	lcd_init();
	
	lcd_cmd(0x01);// clear LCD display
	_delay_ms(2);
	
	lcd_string("ADC value : ");
	
	lcd_goto(1,0);// go to second line
	
	lcd_string("voltage   : ");
	
	float voltage;
	
    while (1) 
    {
		ADCSRA|=(1<<ADSC);// start conversion
		
		while((ADCSRA&(1<<ADSC)));// wait until conversion : ADSC becomes 0 after conversion
		
		value=ADC;// store value
		
		lcd_goto(0,12);
		lcd_string("    ");   // clear old digits
		
		lcd_goto(0,12);
		
		lcd_num(value);
		
		voltage=((float)(value*5))/1023;
		
		lcd_goto(1,12);
		
		lcd_float(voltage);
		
    }
}

