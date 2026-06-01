/*
 * I2C_Baremetal.c
 *
 *  Created on: May 31, 2026
 *      Author: hp
 */


#include "I2C_Baremetal.h"



/*
 * I2C features in STM32F401
 *********************************************************************
 * - Available modes: standard mode(<=100KHz), fast mode(<=400KHz)
 * - optional clock stretching
 * - supports multi-master support through arbitration
 * - 7 bit / 10 bit addressing
 * - dual addressing capability
 * - event flags (SB, ADDR, TXE, RXNE, BTF, BSY)
 * - error flags (ARLO, AF, OVR, BERR)
 */

// I2C Configurations:-
// - Select I2C1 instance
// - Set APB1 frequency in CR2 register
// - Set up SCL clock frequency in CCR register
// - Set maximum allowed rise time in TRISE register
// - GPIO configurations for I2C pins
// - Enable the peripheral

void I2C_Initialization()
{
	// Enable clock for I2C stack
	I2C1_CLK_EN();

	// Set Peripheral clock frequency in CR2_FREQ[5:0] bits
	// - APB1 CLK = HSI = 16MHz
	// - Minimum value that can be set 2MHz
	I2C1->I2C_CR2 &=~(0x3F<< I2C_CR2_FREQ);
	I2C1->I2C_CR2 |= (16<< I2C_CR2_FREQ);

	// Set SCL frequency as 100KHz
	// - Set standard mode I2C_CCR_FS = 0
	// - F_SCL = F_APB1 / 2 x CCR
	// - CRR = 16MHz/ 2x 100KHz = 80
	// - Set in CCR[11:0]
	I2C1->I2C_CCR &=~(1<< I2C_CCR_FS);
	I2C1->I2C_CCR &=~ (0xFFF<< 0);
	I2C1->I2C_CCR |= (80<< 0);

	// Set maximum rise time
	// - In standard mode, maximum allowed rise time is 1000ns
	// - TRISE[5:0] bits = (F_APB1 x maximum rise time) + 1
	// -            = ( 16MHz x 1000ns ) + 1 = 17
	I2C1->I2C_TRISE &=~(0x3F<< 0);
	I2C1->I2C_TRISE |= (17<< 0);

	// Set GPIO Configurations
	// - PB8 and PB9 are used as SCl and SDA of I2C1 instance
	// - Use alternate function AF4
	// - SCL and SDA are open drain with pull up
	GPIOB_CLK_EN();
	GPIOB->MODER  &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->MODER  |= (0x2<< 8*2)|(0x2<< 9*2);
	GPIOB->AFR[1] &=~((0xF<<0*4)|(0xF<< 1*4));
	GPIOB->AFR[1] |= ((0x4<<0*4)|(0x4<< 1*4));
	GPIOB->OTYPER |= (1<< 8)|(1<< 9);
	GPIOB->PUPDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->PUPDR |= ((0x1<< 8*2)|(0x1<< 9*2));
	GPIOB->OSPEEDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->OSPEEDR |=((0x3<< 8*2)|(0x3<< 9*2));

	// Enable I2C peripheral
	I2C1->I2C_CR1 |= (1<< I2C_CR1_PE);

}
void GPIO_Configurations_SCL_SDA()
{

	// Set GPIO Configurations
	// - PB8 and PB9 are used as SCl and SDA of I2C1 instance
	// - Use alternate function AF4
	// - SCL and SDA are open drain with pull up
	GPIOB_CLK_EN();
	GPIOB->MODER  &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->MODER  |= (0x2<< 8*2)|(0x2<< 9*2);
	GPIOB->AFR[1] &=~((0xF<<0*4)|(0xF<< 1*4));
	GPIOB->AFR[1] |= ((0x4<<0*4)|(0x4<< 1*4));
	GPIOB->OTYPER |= (1<< 8)|(1<< 9);
	GPIOB->PUPDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->PUPDR |= ((0x1<< 8*2)|(0x1<< 9*2));
	GPIOB->OSPEEDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->OSPEEDR |=((0x3<< 8*2)|(0x3<< 9*2));

}
// I2C Communication Events:-
// - communication happens through sequence of events
// - includes start, sending address(W/R), sending data, receiving data, stop condition

void generate_start(I2C_Reg_t* I2Cx)
{
	// this signals start of transaction
	// set START bit in CR1 register
	I2Cx->I2C_CR1 |= (1<< I2C_CR1_START);
	// wait till start generation (SR1_SB = HIGH)
	while(!(I2Cx->I2C_SR1 & I2C_SR1_SB));
}
void send_address_write(I2C_Reg_t* I2Cx, uint8_t slv_address)
{
	// send 7 bit address for write operation
	// LSB = 0 for write
	I2Cx->I2C_DR = (slv_address<<1)|0;
	// wait till acknowledgment of address
	while(!(I2Cx->I2C_SR1 & I2C_SR1_ADDR));
}
void clear_ADDR_flag(I2C_Reg_t* I2Cx)
{
	// clearing ADDR flag starts transmission/reception
	// clear ADDR flag after sending address
	// clearing done by reading SR1 and SR2

	(void)I2Cx->I2C_SR1;
	(void)I2Cx->I2C_SR2;
}
void send_data(I2C_Reg_t* I2Cx, uint8_t data)
{
	// wait till transmit buffer empty (I2C_SR1_TXE=HIGH)
	while(!(I2Cx->I2C_SR1 & I2C_SR1_TXE));
	// send data
	I2Cx->I2C_DR = data;
}
uint8_t check_status_flag(I2C_Reg_t* I2Cx, uint32_t flag)
{
	if(I2Cx->I2C_SR1 & flag)
		return 1;
	else
		return 0;
}
void send_address_read(I2C_Reg_t* I2Cx, uint8_t slv_address)
{
	// send address for read operation
	// LSB = 1 for read
	I2Cx->I2C_DR = (slv_address<< 1)|1;

	// wait till slave send back acknowledgment
	while(!(I2Cx->I2C_SR1 & I2C_SR1_ADDR));
}
void set_ACK(I2C_Reg_t* I2Cx)
{
	// setting ACK bit sends acknowledgment after reception of data
	// used for continuous reception of data
	I2Cx->I2C_CR1 |= (1<< I2C_CR1_ACK);
}
void set_NACK(I2C_Reg_t* I2Cx)
{
	// clearing ACK bit sends a NACK after reception of data
	// used for stopping further data reception
	I2Cx->I2C_CR1 &=~(1<< I2C_CR1_ACK);
}
void set_POS(I2C_Reg_t* I2Cx)
{
	// set pos bit to ACK first byte and NACK next byte
	I2Cx->I2C_CR1 |= (1<< I2C_CR1_POS);
}
void clear_POS(I2C_Reg_t* I2Cx)
{
	// clear pos bit
	I2Cx->I2C_CR1 &=~(1<< I2C_CR1_POS);
}
void generate_stop(I2C_Reg_t* I2Cx)
{
	// this signals end of transaction (bus becomes free/idle)
	// set STOP bit in CR1 register
	I2Cx->I2C_CR1 |= (1<< I2C_CR1_STOP);
}
uint8_t read_data(I2C_Reg_t* I2Cx)
{
	// wait till RX buffer not empty
	while(!(I2Cx->I2C_SR1 & I2C_SR1_RXNE));

	// return data in Data register
	return (uint8_t)I2Cx->I2C_DR;
}
void wait_till_transfer(I2C_Reg_t* I2Cx)
{
	// wait till end of transmission/ reception (BTF=1)
	// - data shifted out of the shift register (Transmission)
	// - data received in shift register and unread data present in DR (Reception)
	while(!(I2Cx->I2C_SR1 & I2C_SR1_BTF));
}

void I2C_Application(void)
{
	// set a debug LED
		GPIOA_CLK_EN();
		GPIOA->MODER &=~(0x3<< 0*2);
		GPIOA->MODER |=(0x1<< 0*2);
	 USART_Configuration();


	 // Reading time from a Real-Time Clock
	 // - Slave address of RTC - 0x68
	 // - RTC stores time(sec,min,hr,..) in BCD format
	 // - Address of seconds register - 0x00
	 // - minute is stored in 0x01, hour in 0x02, data in 0x03,...
	 // ---- MSB of byte in 0x00 is kept 0 for RTC ticking (Use mask 0x7F for read/write)
	 // ---- Bit_6 of hours register is used for setting 12/24 hour mode (0 - 24hr, 1- 12hr)
	 // ---- Bit_5 is used in 12-hour mode to indicate AM/PM
	 // ---- Bit_5:0 is used for storing time in 24hour mode (Use mask 0x3F for read/write)

	 // - initialize RTC with current time
	 // - read time from registers

	 I2C_Initialization();

	 // start communication
	 generate_start(I2C1);

	 // send slave address for writing to registers
	 send_address_write(I2C1, 0x68);

	 // clear ADDR flag for starting transmission
	 clear_ADDR_flag(I2C1);

	 // send address of seconds register (0x00)
	 send_data(I2C1, 0x00);

	 // write time to registers
	 // - seconds in 0x00 with MSB=0
	 // - minutes in 0x01
	 // - hours   in 0x02 with Bit_6=0
	 // - write time in BCD format

	 // 55 seconds
	 uint8_t seconds= 55;
	 send_data(I2C1, (((seconds/10)<< 4)| (seconds%10) )& 0x7F);

	 // 24 minutes
	 uint8_t minutes = 24;
	 send_data(I2C1, ((minutes/10)<< 4)|(minutes%10) );

	 // 18 hours (24hour form)
	 uint8_t hours = 18;
	 send_data(I2C1, (((hours/10)<< 4)| (hours%10)) & 0x3F);

	 // wait till end of transfer before stopping communication
	 wait_till_transfer(I2C1);

	 generate_stop(I2C1);


	 DEBUG_LED;

		 // Read time from RTC Registers


		 // start communication
		 generate_start(I2C1);

		 // send slave address for write
		 send_address_write(I2C1, 0x68);

		 // clear ADDR flag for starting transmission
		 clear_ADDR_flag(I2C1);

		 // send address of register to read data
		 send_data(I2C1, 0x00);

		 // Restart communication
		 generate_start(I2C1);

		 // send address for read operation
		 send_address_read(I2C1, 0x68);

		 // set ACK for reading 3 bytes
		 set_ACK(I2C1);

		 // clear ADDR flag for starting reception
		 clear_ADDR_flag(I2C1);


		 // read time from registers
		 // - seconds in 0x00
		 // - minutes in 0x01
		 // - hours   in 0x02
		 // - convert BCD to decimal format

		 // read first two bytes with ACK
		 seconds = (read_data(I2C1)& 0x7F);
		 minutes = read_data(I2C1);

		 // read last byte with NACK
		 set_NACK(I2C1);
		 generate_stop(I2C1);
		 hours = (read_data(I2C1) & 0x3F);


		 // convert BCD to decimal format
		 seconds = (((seconds>>4)*10) + (seconds&0x0F));
		 minutes = ((( minutes>>4)*10) + ( minutes&0x0F));
	     hours  = (((hours >>4)*10) + (hours &0x0F));

	     USART_Send_String("I2C debugging\n");
		 USART_Send_Number(hours);
		 USART_Transmit(USART2,'\n');
		 USART_Send_Number(minutes);
		 USART_Transmit(USART2,'\n');
	     USART_Send_Number(seconds);
		 USART_Transmit(USART2,'\n');




}

















