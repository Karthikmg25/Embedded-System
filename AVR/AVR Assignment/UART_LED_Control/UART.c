/*
 * UART.c
 *
 * Created: 18-05-2026 12:22:13
 *  Author: hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

// Initialize UART
// - Set baud rate of 9600
// - Set Data bits as 8
// - No parity, Single STOP bit
void Init_UART()
{
	// Baud rate setting:
	// - Find UBBR0 value for 9600 Baud rate
	// - Equation: UBRR0 = (F_CPU / 16* Baud )-1
	// - For 9600 Baud, UBRR0 = 103
	UBRR0 = 103;
	
	// Enable RX and TX pins:
	UCSR0B |= (1<< RXEN0)|(1<< TXEN0);
	
	// Set data bits: UCSZ0 2:0 = 011
	UCSR0B &= ~(1<< UCSZ02);
	UCSR0C |= (1<< UCSZ01)|(1<< UCSZ00);
	
	// Clear bit in UCSR0A to avoid doubling baud rate
	UCSR0A &= ~(1<< U2X0);
	
}
// Transmit function
// - Wait till TX buffer is empty
// - Write to data register for transmission
void UART_Send_data(uint8_t data)
{
	while(!(UCSR0A & (1<< UDRE0)));
	
	UDR0 = data;
}
// Receive function
// - Wait till data received in RX buffer
// - Get and return data
uint8_t UART_Receive_data()
{
	while(!(UCSR0A & (1<< RXC0)));
	
	return UDR0 ;
}
// String sending function
void UART_Send_string(char *string)
{
	while(*string)
	{
		UART_Send_data(*string);
		
		string++;
	}
}
// Number sending function
// - Convert number to a string using sprintf
// - Send as string
void UART_Send_number(unsigned int num)
{
	char string[10];
	
	sprintf(string, "%u", num);
	
	UART_Send_string(string);
}
// String receive function
// - receive characters in a loop
// - store characters into the string buffer
// - stops when character is '\n' or '\r' (Enter Key),
// --- stop reception,
// --- provide null termination for string
void UART_Receive_string(char *string, uint8_t buff_size)
{
	uint8_t i=0;
	while(1)
	{
		// Receive new character
		char ch= UART_Receive_data();
		
		// stop reception on enter key
		if(ch == '\n' || ch == '\r')
		{
			break;
		}
		// Check buffer overflow
		// - Write only when buffer space is available
		// - leave space for '\0' character
		if(i < (buff_size-1))
		{
			string[i++] = ch;
		}
		else
		{
			break;
		}
		
	}
	// Null termination
	string[i] = '\0';
}