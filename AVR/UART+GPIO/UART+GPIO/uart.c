/*
 * uart.c
 *
 * Created: 27-12-2025 22:16:29
 *  Author: hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <util/delay.h>

void uart_init()
{
	uint16_t br=103;// br= (F_CPU/(16*9600))-1 , for baud rate = 9600
	
	UBRR0H=br>>8;// upper 8 bits of br
	UBRR0L=br;// lower 8 bits of br
	
	UCSR0B|=(1<<RXEN0)|(1<<TXEN0);// enable RX and TX
	
	UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00);// data format for 8N1- 8 data bits, no parity, 1 stop bit
}

void TX_char(char c)// sending a character
{
	while(!(UCSR0A&(1<<UDRE0)));// wait until UDR0 is empty, else data overwrite
	
	UDR0=c;// writing data enables automatic transmission
}

void TX_string(char *str)// sending a string
{
	while(*str)
	{
		TX_char(*str);// send each character
		
		str++;
	}
	
}
char RX_char()// receiving a character
{
	while(!(UCSR0A&(1<<RXC0)));//wait until data receives
	
	return UDR0;// return character
}

void RX_string(char *buf, uint8_t size)// receiving a string
{
	char c;
	uint8_t i=0;// index
	
	while(1)
	{
		c=RX_char();// receive character
		
		if(c=='\n'|| c=='\r')
		{
			buf[i]='\0';// end of string
			break;
		}
		if(i<size-1)// prevent overflow of array
		{
			buf[i]=c;
			i++;
		}
		else
		{
			buf[i]='\0';// if memory overflows
			break;
		}
	}
	
}

void TX_num(uint16_t num)// sending numbers
{
	char str[6];// 5 digits + '\0'
	
	itoa(num,str,10);// convert to string
	
	TX_string(str);// send string
}

uint16_t RX_num(char *str)// number receiving
{
	uint16_t num=0;
	
	while(*str)
	{
		if(*str>='0' && *str<='9')// if character is valid
		{
			num=num*10 + (*str-'0');// convert character to a digit before calculation
			
		}
		str++;
	}
	return num;
}
void TX_float(float value)// float sending
{
	char buf[10];
	
	dtostrf(value,0,2,buf);// convert float to a string
	
	TX_string(buf);// send the string
	
}

