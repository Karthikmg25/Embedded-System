/*
 * USART_Baremetal.c
 *
 *  Created on: May 27, 2026
 *      Author: hp
 */




#include "USART_Baremetal.h"

/*
 * STM32 USART Features:
 ***********************************************************
 *
 * - Supports synchronous, asynchronous, half-duplex modes
 * - Configurable over sampling rate (16 / 8)
 * - Separate buffers for TX and RX
 * - Hardware flow control using CTS/RTS pins for fast,reliable data transfer
 * - DMA support
 * - Rich set of event flags (RXNE, TXE, TC, OVR, FE, PE, NF)
 */
void USART_Configuration(void)
{
	// Basic USART Configuration for Transmit and Receive
	// Steps:
	// 1) Configure GPIO pins as RX and TX using Alternate function mode
	// 2) Set baud rate in BRR register
	// 3) Enable RX and TX lines
	// 4) Set data bits, stop bits and parity bits
	// 5) Enable UART
	// Use RXNE, TXE, TC status flags and data register for transmission and reception


	// Enable Clock for USART2
	USART2_CLK_EN();


	// 1) Configure GPIO pins as RX and TX lines:
	// - USART2 RX pin is mapped to PA3 (D0)
	// - USART2 TX pin is mapped to PA2 (D1)
	// - Use Alternate function AF7
	// enable GPIO clock
	GPIOA_CLK_EN();
	// set alternate function mode
	GPIOA->MODER &=~((0x3<< 2*2)|(0x3<< 3*2));
	GPIOA->MODER |= (0x2<< 2*2)|(0x2<< 3*2);
	// set alternate function AF7
	GPIOA->AFR[0] &=~((0xF<< 2*4)|(0xF<< 3*4));
	GPIOA->AFR[0] |= (0x7<< 2*4)|(0x7<< 3*4);



	// 2) Set baud rate as 9600:
	// - baud rate = F_CPU / (16 x usart_div)
	// - usart_div = 16MHz / 16 x 9600 = 104.166
	// ---- Integer part of usart_div is stored in upper 12 bits of BRR
	// ---- Fraction part is scaled to 4 bits (multiplication with 16) and stored in lower 4 bits
	// - (Equation changes for x8 over sampling)

	// Use fixed points arithmetic (x100)
	uint32_t usart_div = ((16000000 * 100)/ (16*9600));
	uint32_t integer_part = usart_div / 100;
	uint32_t fraction_part = usart_div % 100;// this is fraction part x 100
	// scale fraction part to 4 bit value (x16), add 50 for precision
	uint32_t scaled = ((fraction_part * 16 ) + 50 )/ 100;

	USART2->USART_BRR = (integer_part<< 4)|(scaled & 0xF);



	// 3) Set data frame format
	// - data bits as 8: CR1[M] = 0
	// - stop bits as 1: CR2->STOP[1:0] = 00
	// - no parity     : CR1[PCE] = 0
	USART2->USART_CR1 &=~(1<< USART_CR1_M);

	USART2->USART_CR2 &=~(0x3<< USART_CR2_STOP);

	USART2->USART_CR1 &=~(1<< USART_CR1_PCE);



	// 4) Enable RX and TX lines
	USART2->USART_CR1 |= (1<< USART_CR1_RE);
	USART2->USART_CR1 |= (1<< USART_CR1_TE);


	// 5) Enable USART
	USART2->USART_CR1 |= (1<< USART_CR1_UE);


	// Transmit, receive APIs
	//void USART_Transmission(uint8_t);
	//uint8_t USART_Reception();

	// USART Test: Echo received bytes


}
void USART_Transmission(uint8_t byte)
{

	// Transmit a single byte:
    // - wait till TX buffer is empty: TXE = 1
	// - write data into data register for transmission
	while(!(USART2->USART_SR & USART_SR_TXE));
	USART2->USART_DR = byte;
}
uint8_t USART_Reception()
{
	// Receive a single byte:
	// - wait till data received in RX buffer: RXNE = 1
	// - read data from data register
	while(!(USART2->USART_SR & USART_SR_RXNE));
	uint8_t byte = USART2->USART_DR ;
	return byte;
}
void USART_Send_String(char* string)
{
	while(*string)
	{
		USART_Transmission(*string);
		string++;
	}
}
void USART_Send_Number(uint8_t number)
{
	char string[10];
	// convert number to string
	itoa(number, string, 10);
	// send number as string
	USART_Send_String(string);
}




