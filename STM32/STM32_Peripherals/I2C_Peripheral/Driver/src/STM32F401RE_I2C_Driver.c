/*
 * STM32F401RE_I2C_Driver.c
 *
 *  Created on: May 31, 2026
 *      Author: hp
 */

#include "STM32F401RE_I2C_Driver.h"


/*
 * Develop a polling based I2C driver
 * Features
 * - supports contoller mode, no slave mode
 * - polling based transmission and reception
 * - supports 7-bit addressing
 * - supports standard mode and fast mode SCL frequencies
 *
 */
void I2C_PeriClockControl(I2C_Reg_t *pI2Cx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
	     if(pI2Cx== I2C1)
		 {
			  I2C1_CLK_EN();
		 }
	     else  if(pI2Cx== I2C2)
	     {
	         I2C2_CLK_EN();
	     }

		 else  if(pI2Cx== I2C3)
		{
			I2C3_CLK_EN();
	    }

	 }
	else
	{
		 if(pI2Cx== I2C1)
	     {
		     I2C1_CLK_DI();
		  }
	      else  if(pI2Cx== I2C2)
		  {
              I2C2_CLK_DI();
	       }

	      else  if(pI2Cx== I2C3)
		   {
			  I2C3_CLK_DI();
	       }

	}
}
// This function Initializes I2C peripheral
// - sets SCL frequency, SCL mode, and duty cycle

void I2C_Init(I2C_Handle_t* pI2C_Handle)
{
	// Enable clock for peripheral
	 I2C_PeriClockControl(pI2C_Handle->I2Cx, ENABLE);

	// Set APB1 frequency in CR2_FREQ[5:0] bits
	pI2C_Handle->I2Cx->I2C_CR2 &=~(0x3F<< 0);
	pI2C_Handle->I2Cx->I2C_CR2 |= (16<< 0);

	uint32_t CCR_Value =0;
	// set SCL frequency and rise time based on speed mode
	if(pI2C_Handle->I2C_Config.I2C_Speed_mode == Standard_Mode)
	{
		// Clear CCR_FS bit
		pI2C_Handle->I2Cx->I2C_CCR &=~(1<< I2C_CCR_FS);

		// Set CCR value based on SCL speed
		// CCR[11:0] = F_APB1/ (2*SCL_speed)
		pI2C_Handle->I2Cx->I2C_CCR &=~(0xFFF<< 0);
		CCR_Value = 16000000/(2*(pI2C_Handle->I2C_Config.I2C_SCL_Speed));
		pI2C_Handle->I2Cx->I2C_CCR |= (CCR_Value&0xFFF);

		// Set maximum rise time
		// - In standard mode, maximum allowed rise time is 1000ns
		// - TRISE[5:0] bits = (F_APB1 x maximum rise time) + 1
		// -            = ( 16MHz x 1000ns ) + 1 = 17
		pI2C_Handle->I2Cx->I2C_TRISE &=~(0x3F<< 0);
		pI2C_Handle->I2Cx->I2C_TRISE |= (17<< 0);


	}
	else if(pI2C_Handle->I2C_Config.I2C_Speed_mode == Fast_Mode)
	{
		// Set CCR_FS bit
		pI2C_Handle->I2Cx->I2C_CCR |= (1<< I2C_CCR_FS);

		// Set Duty cycle
		pI2C_Handle->I2Cx->I2C_CCR &=~(1<< I2C_CCR_DUTY);
		pI2C_Handle->I2Cx->I2C_CCR &=~(pI2C_Handle->I2C_Config.I2C_SCL_DutyCycle<< I2C_CCR_DUTY);

		// SCL speed is set based on Duty cycle
		if(pI2C_Handle->I2C_Config.I2C_SCL_DutyCycle == FastMode_duty_2)
		{
			// CCR value = F_APB1/ (3*SCL_Speed)
			CCR_Value = 16000000/(3*pI2C_Handle->I2C_Config.I2C_SCL_Speed);
			pI2C_Handle->I2Cx->I2C_CCR &=~(0xFFF<< 0);
			pI2C_Handle->I2Cx->I2C_CCR |= (CCR_Value&0xFFF);

		}
		else
		{
			// CCR value = F_APB1/ (25*SCL_Speed)
			CCR_Value = 16000000/(25*pI2C_Handle->I2C_Config.I2C_SCL_Speed);
			pI2C_Handle->I2Cx->I2C_CCR &=~(0xFFF<< 0);
			pI2C_Handle->I2Cx->I2C_CCR |= (CCR_Value&0xFFF);

		}
		// Set maximum rise time
		// - In fast mode, maximum allowed rise time is 300ns
		// - TRISE[5:0] bits = (F_APB1 x maximum rise time) + 1
		// -            = ( 16MHz x 300ns ) + 1 = 4.8 = 5
				pI2C_Handle->I2Cx->I2C_TRISE &=~(0x3F<< 0);
				pI2C_Handle->I2Cx->I2C_TRISE |= (5<< 0);
	}

	// Enable the peripheral
	pI2C_Handle->I2Cx->I2C_CR1 |= (1<< I2C_CR1_PE);


}
// This function transmits a data buffer to the slave
// - Args - I2C instance used
//        - register address to which data is sent
//        - pointer to data buffer
//        - length of data buffer
//        - address of slave device
void I2C_Transmit_Buffer(I2C_Reg_t* I2Cx, uint8_t reg_address, uint8_t* Tx_Buff, uint8_t len, uint8_t slv_address)
{
	// Start communication
	generate_start(I2Cx);

	// Send slave address for transmission
	send_address_write(I2Cx, slv_address);
	// Clear ADDR flag to start transmission
	clear_ADDR_flag(I2Cx);

	// Send register address
	send_data(I2Cx, reg_address);

	// Send data buffer
	while(len>0)
	{
		send_data(I2Cx, *Tx_Buff);
		Tx_Buff++;
		len--;
	}
	// wait till end of trnsmission
	wait_till_transfer(I2Cx);

	// Stop generation
	generate_stop(I2Cx);

}
// This function is used for receiving data into a buffer
// - Args - I2C instance used
//        - register address to read data
//        - pointer to data buffer
//        - length of data to be received
//        - address of slave device
void I2C_Receive_Buffer(I2C_Reg_t* I2Cx, uint8_t reg_address, uint8_t* Rx_Buff, uint8_t len, uint8_t slv_address)
{
	// Start communication
	generate_start(I2Cx);

	// Send slave address for transmission
	send_address_write(I2Cx, slv_address);
	// Clear ADDR flag to start transmission
	clear_ADDR_flag(I2Cx);

	// Send register address
	send_data(I2Cx, reg_address);

	// Generate a restart
	generate_start(I2Cx);

	// Receiving sequence depends on number of receive bytes (len)
	// - 1) len =1 : address_read -> NACK+STOP -> clear ADDR -> wait for RXNE and read data
	// - 2) len =2 : address_read -> NACK+POS -> clear ADDR -> wait for BTF -> STOP -> read both bytes
	// - 2) len >2 : address_read -> ACK -> clear ADDR -> read n-1 bytes -> NACK+STOP -> read last byte

	if(len == 1)// Single byte Reception
	{
		// Send slave address for read operation
		send_address_read(I2Cx, slv_address);

		// set NACK + STOP
		set_NACK(I2Cx);
		generate_stop(I2Cx);

		// clear ADDR flag to start reception
		clear_ADDR_flag(I2Cx);

		// read single byte
		*Rx_Buff = read_data(I2Cx);
	}
	else if(len == 2)// Dual bytes reception
	{
		// Send slave address for read operation
		send_address_read(I2Cx, slv_address);

		// set NACK + POS
		// ACK for the first byte and NACK for the next byte
		set_NACK(I2Cx);
		set_POS(I2Cx);

		// clear ADDR flag to start reception
		clear_ADDR_flag(I2Cx);

		// wait till transfer fiished
		// bytes received in DR and shift register
		wait_till_transfer(I2Cx);

		generate_stop(I2Cx);

		// read both bytes
		*Rx_Buff = I2Cx->I2C_DR;
		Rx_Buff++;
		*Rx_Buff = I2Cx->I2C_DR;

		// clear POS bit for furthur reads
		clear_POS(I2Cx);

	}
	else if(len > 2)// Multi bytes reception
	{
		// Send slave address for read operation
		send_address_read(I2Cx, slv_address);

		// set ACK for n-1 bytes
		set_ACK(I2Cx);

		// clear ADDR flag to start reception
		clear_ADDR_flag(I2Cx);

		// receive n-1 bytes
		len = len-1;
		while(len)
		{
			*Rx_Buff = read_data(I2Cx);
			Rx_Buff++;
			len--;
		}

		// receive last byte with NACK + STOP
		set_NACK(I2Cx);
		generate_stop(I2Cx);


		*Rx_Buff = read_data(I2Cx);


	}





}








