/*
 * uart_basics.c
 *
 * Created: 31-12-2025 21:01:03
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void UART_init()
{
	UBRR0H=0;
	UBRR0L=103;// for 960000 baud rate
	
	UCSR0B|=(1<<TXEN0)|(1<<RXEN0);// enable RX and TX
	
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);// data format 8N1- 8bit, no parity, 1 stop bit
	
	UCSR0C&=~((1<<UPM00)|(1<<UPM01)|(1<<USBS0));// optional ,no parity,1 stop bit
	
}
void Transmit_char(char c)
{
	while(!(UCSR0A&(1<<UDRE0)));// wait till UDR0 is empty
	UDR0=c;// writing enables automatic transmission
}

void transmit_string(char *str)
{
	while(*str)
	{
		Transmit_char(*str);// send each characters
		str++;
	}
}

int main(void)
{
	char ch='A';
	char str[]="Hello";
    UART_init();
	
    while (1) 
    {
		Transmit_char(ch);// send character
		Transmit_char('\n');// next line
		
		transmit_string(str);// send string
		Transmit_char('\n');// next line
		
		_delay_ms(1000);
    }
}

