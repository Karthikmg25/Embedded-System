/*
 * SPI.c
 *
 * Created: 19-05-2026 13:26:35
 *  Author: hp
 */ 

#include <avr/io.h>
#include "SPI.h"

// Initialize SPI
// - select master mode
// - set SCK frequency
// - set clock mode 
// - enable SPI
void Init_SPI()
{
	// Master mode
	SPCR |= (1<< MSTR);
	// SCK frequency as 1MHz: SPR1:0 = 01
	SPCR |= (1<< SPR1)|(1<< SPR0);
	// set clock mode as 00
	SPCR &= ~((1<< CPOL)|(1<<CPHA));
	// Enable SPI
	SPCR |= (1<< SPE);
}
// Initialize SPI pins
// In master mode:
// - set MISO as input
// - set all others as output
// - set SS pin as HIGH
void Init_SPI_Pins()
{
	DDRB |= (1<<MOSI_PIN)| (1<<SCK_PIN)|(1<<SS_PIN);
	SPI_PORT |= (1<<SS_PIN);
	
	DDRB &= ~(1<< MISO_PIN);
}
// Function for data transfer
// Data write generates clock
// Data bits transmitted and received per CLK pulse (Full Duplex)
// SPIF is set when a transfer ends
uint8_t SPI_transfer(uint8_t data)
{
	// write data to SPDR
	SPDR = data;
	
	// wait till transfer completes
	while(!(SPSR & (1<< SPIF)));
	
	// Get received data
	return SPDR;
}
// Start communication by selecting slave
// Select slave by pulling SS_PIN LOW
void Select_slave()
{
	SPI_PORT &= ~(1<< SS_PIN);
}

// Stop communication by De-selecting slave : SS_PIN = 1
void Deselect_slave()
{
	SPI_PORT |= (1<< SS_PIN);
}