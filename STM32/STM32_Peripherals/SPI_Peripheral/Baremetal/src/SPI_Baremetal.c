/*
 * SPI_Baremetal.c
 *
 *  Created on: May 29, 2026
 *      Author: hp
 */


#include "SPI_Baremetal.h"

/*
 * STM32 SPI Features:
 ***********************************************************
 *
 * - Supports synchronous, full-duplex, half-duplex, simplex communication modes
 * - Hardware and software slave management
 * - Configurable data frame ( 8bits / 16bits )
 * - Configurable data order ( MSB / LSB first)
 * - Hardware Cyclic Redundancy Check (CRC) for reliable communication
 * - Multi-master communication possible (less common)
 * - DMA support
 * - Rich set of event flags and interrupts for event-driven communication
 */
void SPI_Configuration(void)
{
	// SPI Configurations for master mode communication
	//////////////////////////////////////////////////////
	// 1) Select Master mode
	// 2) Set SPI clock frequency
	// 3) Set clock phase and clock polarity
	// 4) Set data frame format / data size
	// 5) Enable Software slave management (SSM) and set SSI
	// 6) Configure GPIO pins for SPI alternate functions
	// 7) Enable SPI
	// Optional: - set data order (MSB first /LSB first)
	//           - set bidirectional mode
	//           - set CRC check

	// Enable SPI clock: Use SPI1
	SPI1_CLK_EN();

	// Master mode selection
	SPI1->SPI_CR1 |= (1<< SPI_CR1_MSTR);

	// SCK frequency: 1MHz
	// - set prescaler 16
	// - SPI_CR1_BR[2:0] = 011
	SPI1->SPI_CR1 &=~(0x7<< SPI_CR1_BR);
	SPI1->SPI_CR1 |= (0x3<< SPI_CR1_BR);

	// Set data frame format: 8 bits
	// - CR1_CR1[DFF] = 0
	SPI1->SPI_CR1 &=~(1<< SPI_CR1_DFF);

	// Set clock polarity and phase
	// - CPOL = 0: CLK is LOW when idle
    // - CPHA = 0: data sampled on first clock edge
	SPI1->SPI_CR1 &=~((1<< SPI_CR1_CPOL)|(1<< SPI_CR1_CPHA));

	// Set software slave management
	// - slave select is controlled using software
	// - any GPIO Output pin can be configured as SS pin
	// - SSI is set as HIGH in this mode
    // - CR1[SSM] = 1 && CR1[SSI] = 1
	SPI1->SPI_CR1 |= (1<< SPI_CR1_SSM)|(1<< SPI_CR1_SSI);

	// Set GPIO pins for SPI alternate function
	SPI_GPIO_Configurations();

	// SPI enable
	SPI1->SPI_CR1 |= (1<< SPI_CR1_SPE);


}
void SPI_GPIO_Configurations()
{
	// GPIO Configuration
	// - SPI1_MOSI -> PA7 (D11)
	// - SPI1_MISO -> PA6 (D12)
	// - SPI1_SCK  -> PA5 (D13)
	// - SPI1_SS   -> PB6 (D10)
	// - use alternate function AF5
	// - set SS pin as GPIO output for software toggle
	// - set SS as default HIGH (inactive)
	// - set HIGH speed (SPI is fast)
	GPIOA_CLK_EN();
	GPIOB_CLK_EN();
	GPIOA->MODER   &= ~((0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2));
	GPIOA->MODER   |= (0x2<< 7*2)| (0x2<< 6*2)|(0x2<< 5*2);
	GPIOB->MODER   &= ~(0x3<< 6*2);
	GPIOB->MODER   |= (1<< 6*2);
	GPIOB->ODR     |= (1<< 6);
	GPIOA->AFR[0]  &= ~((0xF<< 7*4)|(0xF<< 6*4)|(0xF<< 5*4));
	GPIOA->AFR[0]  |= (0x5<< 7*4)|(0x5<< 6*4)|(0x5<< 5*4);
	GPIOA->OSPEEDR &= ~((0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2));
	GPIOA->OSPEEDR |= (0x3<< 7*2)| (0x3<< 6*2)|(0x3<< 5*2);
	GPIOA->PUPDR &=~((0x3<< 5*2)|(0x3<< 6*2)|(0x3<< 7*2));
}
void SPI_Loopback_Test()
{
	// Set a Debug LED
	GPIOA->MODER &= ~(0x3<< 0*2);
	GPIOA->MODER |= (1<< 0*2);


	// SPI Loop-back Test
	// - connect MOSI and MISO lines
	// - transmit a byte (0xFA) via MOSI
	// - receive it via MISO
	// - check data received is same as transmitted

	// wait till TX buffer is empty
	while(!(SPI1->SPI_SR & SPI_SR_TXE));
	// write data to DR
	SPI1->SPI_DR = 0xFA;
	// wait till data received in RX buffer
	while(!(SPI1->SPI_SR & SPI_SR_RXNE));
	// read data from DR
	uint8_t RX_data = SPI1->SPI_DR;

	// check if both are same using debug LED
	if(RX_data == 0xFA)
	{
		// Turn ON debug LED
		GPIOA->ODR |= (1<< 0);
	}
}

// Transmit and receive APIs
///////////////////////////////////////////////////////////////////
// - SPI is inherently full-duplex:
// - every transmission simultaneously receives data,
// - and every reception requires transmission of data
// - slave select and de-select are done outside APIs

/*
 * SPI Transmission
 * - start communication by slave selecting the slave (pull SS LOW)
 * - wait till transmit buffer is empty (TXE = 1)
 * - write to data register for transmission
 * - wait till data received (RXNE = 1)
 * - clear RXNE flag by reading data register (prevents overrun issue)
 * - wait till end of transmission (BSY = 0)
 * - end communication by de-selecting slave (pull SS HIGH)
 *
 */
void SPI_Transmit_Byte(uint8_t byte)
{
    // wait till transmit buffer is empty
	while(!(SPI1->SPI_SR & SPI_SR_TXE));
    // write data for sending
	SPI1->SPI_DR = byte;
    // wait till data received in RX buffer
	while(!(SPI1->SPI_SR & SPI_SR_RXNE ));
	// dummy read clearing RXNE flag
	(void)SPI1->SPI_DR;
	// wait till transmission complete
	while(SPI1->SPI_SR & SPI_SR_BSY);

}

/*
 * SPI Reception
 * - start communication by slave selecting the slave (pull SS LOW)
 * - wait till transmit buffer is empty (TXE = 1)
 * - write a dummy byte to DR for generating SPI clock pulses
 * - wait till RX buffer not empty
 * - read data from DR (clears RXNE)
 * - wait till end of transmission (BSY = 0)
 * - end communication by de-selecting slave (pull SS HIGH)
 */
uint8_t SPI_Receive_Byte()
{
    // wait till transmit buffer is empty
	while(!(SPI1->SPI_SR & SPI_SR_TXE));
    // write dummy byte
	SPI1->SPI_DR = 0xFF;
    // wait till data received in RX buffer
	while(!(SPI1->SPI_SR & SPI_SR_RXNE ));
	// read data form DR
	uint8_t data = SPI1->SPI_DR;
	// wait till transmission complete
	while(SPI1->SPI_SR & SPI_SR_BSY);

	return data;
}


















