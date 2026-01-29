/*
 * LM_35_temperature_sensor.c
 *
 * Created: 23-12-2025 21:02:27
 * Author : hp
 */ 
#define F_CPU 16000000UL
#define EN_PORT PORTB
#define RS_PORT PORTB
#define EN PORTB3
#define RS PORTB4

#include <util/delay.h>
#include <avr/io.h>
#include <stdlib.h>

void ADC_setup(uint8_t channel)// pass channel as argument
{
	ADMUX|=(1<<REFS0);// set reference voltage as VCC
	
	ADMUX=(ADMUX&0xF0)|(channel&0x07);// limit channel value to 7
	
	ADCSRA|=(1<<ADEN);// enable ADC
	
	ADCSRA|=(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);// set pre scalar 128, ADC frequency =125KHz
	
	
}
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
	
	EN_PORT&=~(1<<EN);// set EN low for safety
	
	
	lcd_nibble(0x03);// force 8 bit mode initially
	_delay_ms(5);
	
	lcd_nibble(0x03);// LCD will be in an unknown state at the start
	_delay_us(150);
	
	lcd_nibble(0x03);
	_delay_us(150);
	
	
	lcd_nibble(0x02);// force 4 bit mode
	
	lcd_cmnd(0x28);// function set - 4 bit mode, 2 line display, 5*7 pixel
	
	lcd_cmnd(0x0C);// display ON
	
	lcd_cmnd(0x06);// Entry mode
	
	lcd_clear();
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
	
	if(row==0)
	{
		lcd_cmnd(0x80+(col&0x0F));
	}
	else
	{
		lcd_cmnd(0xC0+(col&0x0F));
	}
}
void lcd_num(uint16_t num)
{
	char str[6];// 5 digits + '\0'
	
	itoa(num,str,10);// convert num to a string
	
	lcd_string(str);
}
void lcd_float(float value)
{
	uint16_t t=(uint16_t)(value*100);
	
	lcd_num(t/100);// integer part
	
	lcd_data('.');// decimal point
	
	if(t%100<10) lcd_data('0');// pad zero
	
	lcd_num(t%100);// fraction part
	
}
int main(void)
{
	DDRD=0xF0;// set PD7-PD4 as outputs
	DDRB|=(1<<EN)|(1<<RS);// set RS and EN as outputs
	
    ADC_setup(0);
	
	lcd_init();
	
	lcd_cmnd(0x01);// clear display
	_delay_ms(2);
	
	lcd_string("Temperature ");
	
	uint16_t adc_value;
	float temp;
    while (1) 
    {
		ADCSRA|=(1<<ADSC);// start conversion
		
		while(ADCSRA&(1<<ADSC));// wait till conversion
		
		adc_value=ADC;
		
		temp=(adc_value*5.0*100)/1023;// convert ADC value to temperature
		
		lcd_goto(1,0);// second line
		
		lcd_string("      ");// clear before printing new value
		
		lcd_goto(1,0);
		
		lcd_float(temp);// print temperature
		
		lcd_data(0xDF);// printing custom character
		lcd_string(" C");
		_delay_ms(500);// ADC stable readings
		
    }
}

