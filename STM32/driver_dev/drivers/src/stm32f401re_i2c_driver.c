/*
 * stm32f401re_i2c_driver.c
 *
 *  Created on: Mar 14, 2026
 *      Author: hp
 *
 */

#include "stm32f401re_i2c_driver.h"

/*************************************************************************
 * @fn                - I2C_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given I2C Peripheral
 *
 * @param[in]         - pI2Cx: base address of the I2C peripheral
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - none
 *
 * @Note              - none
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
/*************************************************************************
 * @fn                - I2C_PerpheralControl
 *
 * @brief             - This function Enables or Disables a I2C Peripheral
 *
 * @param[in]         - pI2Cx
 * @param[in]         - EnorDi
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_PeripheralControl(I2C_Reg_t *pI2Cx, uint8_t EnorDi)
{

	if(EnorDi== ENABLE)
	{
		pI2Cx->I2C_CR1 |= (1<<I2C_CR1_PE);
	}
	else
	{
		pI2Cx->I2C_CR1 &=~ (1<<I2C_CR1_PE);
	}
}
/*************************************************************************
 * @fn                - I2C_Init
 *
 * @brief             - This function initializes I2C with required configurations
 *
 * @param[in]         - pI2Cx_Handle
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_Init(I2C_Handle_t *pI2C_Handle)
{
	I2C_PeriClockControl(pI2C_Handle->pI2Cx, ENABLE);                                                   /* ENABLE PERIPHERAL CLOCK */

	pI2C_Handle->pI2Cx->I2C_CR2 &= ~(0X3F << I2C_CR2_FREQ );// CLEAR 6 BITS                             /* SET CR2 BITS 5:0 WITH PERIPHERAL CLK FREQUENCY */
	pI2C_Handle->pI2Cx->I2C_CR2 |= (((F_PCLK /1000000)&0X3F) << I2C_CR2_FREQ );// 16MHz

	pI2C_Handle->pI2Cx->I2C_OAR1 |= (1<< 14);// always keep 14th bit HIGH : sends ACK when a master calls
	pI2C_Handle->pI2Cx->I2C_OAR1 &=~(0X7F << I2C_OAR1_ADD);// CLAER BITS                                /* SETS DEVICE ADDRESS : SLAVE MODE */
	pI2C_Handle->pI2Cx->I2C_OAR1 |= (pI2C_Handle->I2C_Config.I2C_Device_Address<< I2C_OAR1_ADD);

	uint32_t ccr=0;
	uint32_t tempreg=0;
                                                                                                        /* SET SCL FREQUENCY*/
	if(pI2C_Handle->I2C_Config.I2C_SCL_Speed <= I2C_SCL_SPEED_SM)
	{
		// standard speed

		ccr= F_PCLK /( 2 * pI2C_Handle->I2C_Config.I2C_SCL_Speed);

		pI2C_Handle->pI2Cx->I2C_CCR = ccr & 0xFFF;
	}
	else
	{
		// Fast mode

		 tempreg |= (1<<I2C_CCR_FS);// set fast mode bit

		 tempreg |= (pI2C_Handle->I2C_Config.I2C_SCL_DutyCycle<<I2C_CCR_DUTY);// set duty cycle

		if(pI2C_Handle->I2C_Config.I2C_SCL_DutyCycle ==  I2C_FM_DUTY_2 )// duty cycle 2:1
		{
			ccr= F_PCLK / ( 3 * pI2C_Handle->I2C_Config.I2C_SCL_Speed);

		}
		else                                                           // duty cycle 16:9
		{
			ccr= F_PCLK /( 25 * pI2C_Handle->I2C_Config.I2C_SCL_Speed);

		}
		tempreg |= ccr & 0xFFF;

		pI2C_Handle->pI2Cx->I2C_CCR = tempreg ;

	}

	uint32_t  rise_time=0;
	                                                                                                  /* SET MAXIMUM RISE TIME */
	    if(pI2C_Handle->I2C_Config.I2C_SCL_Speed <= I2C_SCL_SPEED_SM)
		{
			// standard speed : Maximum allowed rise time = 1000ns

		    rise_time = (F_PCLK /1000000) + 1;

		    pI2C_Handle->pI2Cx->I2C_TRISE = rise_time & 0x3F;// Bits 5:0

		}
	    else
	    {
	    	// Fast mode : maximum allowed rise time = 300ns

	    	rise_time = ((F_PCLK * 300)/1000000000) + 1;

	    	pI2C_Handle->pI2Cx->I2C_TRISE = rise_time & 0x3F;

	    }

}
/*
 *  I2C Helper Functions : Define as static for file level enumeration : only used for this file
 */

static void I2C_Generate_Start(I2C_Reg_t* pI2Cx)
{
	pI2Cx->I2C_CR1 |= (1<<I2C_CR1_START); // START

	while(!(pI2Cx->I2C_SR1 & I2C_SR1_SB ));// WAIT TILL START
}
static void I2C_Send_Address_Write(I2C_Reg_t* pI2Cx, uint8_t slv_addr)
{
	slv_addr=slv_addr<<1;
	slv_addr &=~(1); // R/W Bit=0 for write

	pI2Cx->I2C_DR = slv_addr;
	while(!(pI2Cx->I2C_SR1 & I2C_SR1_ADDR));// wait till Address match
}
static void I2C_Clear_ADDR_Flag(I2C_Reg_t* pI2Cx)
{
	(void)pI2Cx->I2C_SR1;
	(void)pI2Cx->I2C_SR2;// read both status registers to clear ADDR flag
}
static void I2C_Send_Byte(I2C_Reg_t* pI2Cx, uint8_t byte)
{
	while(!(pI2Cx->I2C_SR1 & I2C_SR1_TxE ));// WAIT TILL TX BUFFER EMPTY

	pI2Cx->I2C_DR = byte;
}
static void I2C_Send_Address_Read(I2C_Reg_t* pI2Cx, uint8_t slv_addr)
{
	slv_addr=slv_addr<<1;
	slv_addr |= 1;// R/W bit = 1 for read

	pI2Cx->I2C_DR = slv_addr;
	while(!(pI2Cx->I2C_SR1 & I2C_SR1_ADDR));// wait till Address match
}
static void I2C_Set_ACK(I2C_Reg_t* pI2Cx)
{
	pI2Cx->I2C_CR1 |= (1<<I2C_CR1_ACK);// send ACK after reception
}
static void I2C_Set_NACK(I2C_Reg_t* pI2Cx)
{
	pI2Cx->I2C_CR1 &=~(1<<I2C_CR1_ACK);// send NACK after reception
}
static uint8_t I2C_Read(I2C_Reg_t* pI2Cx)
{
	while(!(pI2Cx->I2C_SR1 & I2C_SR1_RxNE));// wait till data received in DR

	uint8_t data = pI2Cx->I2C_DR;

	return data;
}
static void I2C_Wait_Transmission_Complete(I2C_Reg_t *pI2Cx)
{
	while(!(pI2Cx->I2C_SR1 & I2C_SR1_BTF));// wait till data received in DR and Shift register : Reception
	                                       // wait till data end of transmission               : Transmission
}
static void I2C_Set_POS(I2C_Reg_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= (1<< I2C_CR1_POS);// for dual bytes reception ACK/NACK control
}
static void I2C_Clear_POS(I2C_Reg_t *pI2Cx)
{
	pI2Cx->I2C_CR1 &=~(1<< I2C_CR1_POS);
}
static void I2C_Generate_Stop(I2C_Reg_t *pI2Cx)
{
	pI2Cx->I2C_CR1 |= (1<<I2C_CR1_STOP); // Generate Stop after byte transfer
}
/*************************************************************************
 * @fn                - I2C_Transmit_Byte
 *
 * @brief             - This function Transmits a single byte From Master to register of a Slave
 *
 * @param[in]         - pI2Cx    : I2C peripheral
 * @param[in]         - slv_addr : 7-bit Slave Address
 * @param[in]         - Reg_addr : Register address in slave  to write
 * @param[in]         - byte     : Byte to send
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_Transmit_Byte(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t byte)
{
	I2C_Generate_Start( pI2Cx);

	I2C_Send_Address_Write( pI2Cx, slv_addr);

	I2C_Clear_ADDR_Flag(pI2Cx);

	I2C_Send_Byte( pI2Cx, Reg_addr);

	I2C_Send_Byte( pI2Cx, byte);

	I2C_Wait_Transmission_Complete(pI2Cx);

	I2C_Generate_Stop( pI2Cx);

}
/*************************************************************************
 * @fn                - I2C_Transmit_Buffer
 *
 * @brief             - This function Transmits a data buffer From Master to register of a Slave
 *
 * @param[in]         - pI2Cx     : I2C peripheral
 * @param[in]         - slv_addr  : 7-bit Slave Address
 * @param[in]         - Reg_addr  : Register address in slave to Start writing
 * @param[in]         - pTxBuffer : Pointer to Transmit buffer
 * @param[in]         - Tx_len    : Length of data to be transmitted (number of bytes)
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_Transmit_Buffer(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t *Tx_Buffer, uint8_t Tx_Len)
{
	I2C_Generate_Start( pI2Cx);

	I2C_Send_Address_Write( pI2Cx, slv_addr);

	I2C_Clear_ADDR_Flag(pI2Cx);

	I2C_Send_Byte( pI2Cx, Reg_addr);

	while(Tx_Len>0)
	{
		I2C_Send_Byte( pI2Cx, *Tx_Buffer );

		Tx_Buffer++;
		Tx_Len--;
	}

	I2C_Wait_Transmission_Complete(pI2Cx);

	I2C_Generate_Stop( pI2Cx);
}
/*************************************************************************
 * @fn                - I2C_Receive_Byte
 *
 * @brief             - This function receives a single byte from a register of a Slave
 *
 * @param[in]         - pI2Cx    : I2C peripheral
 * @param[in]         - slv_addr : 7-bit Slave Address
 * @param[in]         - Reg_addr : Register address in slave to read from
 *
 * @return            - uint8_t  : data received  from slave-register
 *
 * @Note              - none
 */
uint8_t I2C_Receive_Byte(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr)
{
	I2C_Generate_Start( pI2Cx);                             /* GENERATE START */

	I2C_Send_Address_Write( pI2Cx, slv_addr);               /* SEND SLAVE ADDRESS WRITE*/

	I2C_Clear_ADDR_Flag(pI2Cx);                             /* CLEAR ADDR FLAG */

	I2C_Send_Byte( pI2Cx, Reg_addr);                        /* SEND REGISTER ADDRESS TO SET POINTER */

	I2C_Generate_Start( pI2Cx);                             /* RESTART */

	I2C_Send_Address_Read( pI2Cx, slv_addr);                /* SEND SLAVE ADDRESS READ*/

	I2C_Set_NACK( pI2Cx);                                   /* SET NACK BEFORE RECEPTION PHASE : MEANS NACK AFTER RECEIVING BYTE*/

	I2C_Clear_ADDR_Flag(pI2Cx);                             /* CLEARING ADDR FLAG STARTS RECEPTION PHASE*/

	I2C_Generate_Stop( pI2Cx);                              /* STOP AFTER NEXT BYTE*/

	uint8_t data = I2C_Read( pI2Cx);                        /* READ AFTER RXNE HIGH*/

	return data;

}
/*************************************************************************
 * @fn                - I2C_Recieve_Buffer
 *
 * @brief             - This function receives data from Slave and stores into a buffer
 *
 * @param[in]         - pI2Cx
 * @param[in]         - slv_addr  : 7-bit Slave Address
 * @param[in]         - Reg_addr  : Register address in slave to start reading from
 * @param[in]         - pRxBuffer : Pointer to receive buffer
 * @param[in]         - Rx_len    : Length of data to be received (number of bytes)
 *
 * @return            - none
 *
 * @Note              - none
 */
void I2C_Receive_Buffer(I2C_Reg_t *pI2Cx, uint8_t slv_addr,uint8_t Reg_addr, uint8_t *Rx_Buffer, uint8_t Rx_Len)
{
	if(Rx_Len == 1)                                                        // single byte receive
	{
		*Rx_Buffer =  I2C_Receive_Byte( pI2Cx, slv_addr, Reg_addr);
	}
	else if(Rx_Len == 2)                                                    // Dual byte receive
	{
		I2C_Generate_Start( pI2Cx);                                     /* GENERATE START */

		I2C_Send_Address_Write( pI2Cx, slv_addr);                       /* SEND SLAVE ADDRESS WRITE*/

		I2C_Clear_ADDR_Flag(pI2Cx);                                     /* CLEAR ADDR FLAG */

		I2C_Send_Byte( pI2Cx, Reg_addr);                                /* SEND REGISTER ADDRESS TO SET POINTER */

		I2C_Generate_Start( pI2Cx);                                     /* RESTART */

		I2C_Send_Address_Read( pI2Cx, slv_addr);                        /* SEND SLAVE ADDRESS READ*/

		I2C_Set_NACK( pI2Cx);
		I2C_Set_POS(pI2Cx);                                             /* SET NACK AND POS BEFORE RECEPTION PHASE :
		                                                                   MEANS ACK FOR FIRST BYTE AND NACK AFTER SECOND BYTE*/

		I2C_Clear_ADDR_Flag(pI2Cx);                                     /* START RECEPTION PHASE*/

		I2C_Generate_Stop( pI2Cx);                                      /* STOP AFTER TWO BYTES*/

		I2C_Wait_Transmission_Complete(pI2Cx);                          /* WAIT TILL BOTH BYTES RECEIVED (IN DATA REGISTER AND SHIFT REGISTER)*/

		*Rx_Buffer = pI2Cx->I2C_DR; Rx_Buffer++;
		*Rx_Buffer = pI2Cx->I2C_DR;                                     /* READ BOTH BYTES ,STORE TO BUFFER*/

		I2C_Clear_POS( pI2Cx);                                          /* CLEAR POS FOR FUTURE READS : NORMAL ACK/NACK TIMING*/
	}
	else if(Rx_Len >2)                                                         // Multi-Byte receive
	{
		I2C_Generate_Start( pI2Cx);                                     /* GENERATE START */

		I2C_Send_Address_Write( pI2Cx, slv_addr);                       /* SEND SLAVE ADDRESS WRITE*/

		I2C_Clear_ADDR_Flag(pI2Cx);                                     /* CLEAR ADDR FLAG */

		I2C_Send_Byte( pI2Cx, Reg_addr);                                /* SEND REGISTER ADDRESS TO SET POINTER */

		I2C_Generate_Start( pI2Cx);                                     /* RESTART */

		I2C_Send_Address_Read( pI2Cx, slv_addr);                        /* SEND SLAVE ADDRESS READ*/

		I2C_Set_ACK( pI2Cx);                                            /* SET ACK BEFORE RECEPTION PHASE : MEANS READING INITIAL BYTES WITH ACK*/

		I2C_Clear_ADDR_Flag(pI2Cx);                                     /* START RECEPTION PHASE*/

		while((Rx_Len-1)>0)                                             /* READ TILL SECOND TO LAST BYTE*/
		{
			*Rx_Buffer = I2C_Read( pI2Cx);                              /*READ AFTER RXNE HIGH*/

			Rx_Buffer++;
			Rx_Len--;
		}

		I2C_Set_NACK( pI2Cx);
		I2C_Generate_Stop( pI2Cx);                                      /* SET NACK & STOP BEFORE LAST BYTE */

		*Rx_Buffer = I2C_Read( pI2Cx);                                  /* READ LAST BYTE WITH NACK*/

	}
}




