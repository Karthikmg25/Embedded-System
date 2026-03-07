/*
 * stm32f401re_spi_driver.c
 *
 *  Created on: Feb 23, 2026
 *      Author: hp
 */
#include "stm32f401re_spi_driver.h"

/*************************************************************************
 * @fn                - SPI_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given SPI Peripheral
 *
 * @param[in]         - pSPIx: base address of the SPI peripheral(SPI1, SPI2,SPI3,SPI4)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_PeriClockControl(SPI_Reg_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
	        if(pSPIx== SPI1)
              {
	        	SPI1_CLK_EN();
              }
	  else  if(pSPIx== SPI2)
	          {
		           SPI2_CLK_EN();
	          }

	  else  if(pSPIx== SPI3)
	          {
		           SPI3_CLK_EN();
	          }
	  else  if(pSPIx== SPI4)
	          {
		          SPI4_CLK_EN();
	          }

	}
	else
	{
		          if(pSPIx== SPI1)
		              {
		        	  SPI1_CLK_DI();
		              }
			  else  if(pSPIx== SPI2)
			          {
				         SPI2_CLK_DI();
			          }

			  else  if(pSPIx== SPI3)
			          {
				           SPI3_CLK_DI();
			          }
			  else  if(pSPIx== SPI4)
			          {
				          SPI4_CLK_DI();
			          }
	}

}
/*************************************************************************
 * @fn                - SPI_PerpheralControl
 *
 * @brief             - This function Enables or Disables an SPI Peripheral
 *
 * @param[in]         - pSPIx
 * @param[in]         - EnorDi
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_PeripheralControl(SPI_Reg_t *pSPIx, uint8_t EnorDi)
{
	if(EnorDi == ENABLE)
	{
		pSPIx->SPI_CR1 |= (1<<SPI_CR1_SPE);
	}
	else
	{
		pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_SPE);
	}
}
/*************************************************************************
 * @fn                - SPI_Init
 *
 * @brief             - This function initialize an SPI Peripheral
 *
 * @param[in]         - pSPIHandle
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_Init(SPI_Handle_t *pSPIHandle)
{
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);                                              /* ENABLE CLOCK FOR GIVEN SPI */

	                                                                                               /* SET DEVICE MODE */
	if(pSPIHandle->SPI_Config.SPI_Device_Mode== SPI_DEVICE_MODE_MASTER )
	{
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_MSTR);
	}
	else
	{
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_MSTR);
	}
	                                                                                               /* SELECT SPI CLOCK MODE (CPOL & CPHA)*/
	if(pSPIHandle->SPI_Config.SPI_CPHA == SPI_CPHA_HIGH)
	{
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_CPHA);
	}
	else
	{
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_CPHA);
	}
	if(pSPIHandle->SPI_Config.SPI_CPOL == SPI_CPOL_HIGH)
		{
			pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_CPOL);
		}
		else
		{
			pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_CPOL);
		}

	                                                                                             /* SELECT BUS CONFIGURATION : FULL OR HALF DUPLEX*/
	if(pSPIHandle->SPI_Config.SPI_Bus_Config == SPI_BUS_CONFIG_FD )
	{
		// FULL DUPLEX : MOSI AND MISO ACTIVE
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_BIDIMODE);
	}
	else
		if(pSPIHandle->SPI_Config.SPI_Bus_Config == SPI_BUS_CONFIG_HD )
			{
			     // HALF DUPLEX : ONLY MOSI USED AS SINGLE LINE, MISO IGNORED
				pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_BIDIMODE);
			}   // DATA DIRACTION DEPENDS ON BIDIOE BIT : INPUT OR OUTPUT
		else
		 if(pSPIHandle->SPI_Config.SPI_Bus_Config == SPI_BUS_CONFIG_SIMPLEX_RX )
			{
			   pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_BIDIMODE);
			   pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_RXONLY);
			   // SIMPLEX : ONLY MISO USED FOR INPUT, M0SI IGNORED
			}

	                                                                                            /* SELECT DATA FRAME FORMAT */
	if(pSPIHandle->SPI_Config.SPI_DataFrame == SPI_DFF_8BIT )
	{    // 8 bit
		pSPIHandle->pSPIx->SPI_CR1 &=~(1<<SPI_CR1_DFF);
	}
	else
	{   // 16 bit
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_DFF);
	}
	                                                                                           /* SOFTWARE SLAVE MANAGEMENT*/
	if(pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_EN)
	{
		// SLAVE SELECTED VIA SOFTWARE
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSM);
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSI);
		 // SSI MUST BE HIGH IN SSM + MASTER MODE, ELSE MODE FAULT
	}
	else
	{
		// SLAVE SELECTED VIA HARDWARE
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_SSM);
	}
	                                                                                            /* SET PRESCALAR*/
	pSPIHandle->pSPIx->SPI_CR1 &=~ (0x7 << SPI_CR1_BR);// CLEAR BITS
	pSPIHandle->pSPIx->SPI_CR1 |= (pSPIHandle->SPI_Config.SPI_CLK_Speed << SPI_CR1_BR);
	                                                                                            /* SPI ENABLE DONE SEPERATELY AFTER CONFIGURATIONS*/

}
/*************************************************************************
 * @fn                - SPI_Get_FlagStatus
 *
 * @brief             - This function returns current status of Flag
 *
 * @param[in]         - pSPIx
 * @param[in]         - Flag
 *
 * @return            - none
 *
 * @Note              - none
 */
uint8_t SPI_Get_FlagStatus(SPI_Reg_t *pSPIx, uint8_t Flag)
{
	uint8_t status = pSPIx->SPI_SR & (1<<Flag);

	if(status == 0)
	{
		return RESET;
	}
	return SET;
}
/*************************************************************************
 * @fn                - SPI_Transmit
 *
 * @brief             - This function Transmits data From Master to Slave
 *
 * @param[in]         - pSPIx
 * @param[in]         - pTxBuffer : Pointer to Transmit buffer
 * @param[in]         - Tx_len    : Length of data to be transmitted (number of bytes)
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_Transmit(SPI_Reg_t *pSPIx, uint8_t *pTxBuffer, uint8_t Tx_len)
{
	while(Tx_len>0)
	{
		                                                                    /* WAIT TILL TRANSMIT BUFFER IS EMPTY ( TXE IS SET )                */
		while(!(pSPIx->SPI_SR&(1<<SPI_SR_TXE)));

		                                                                    /* WRITE DATA TO DATA REGISTER FOR TRANSMISSION ( BASED ON DFF )    */
		if(pSPIx->SPI_CR1&(1<<SPI_CR1_DFF))// 16 BIT FRAME
		{
			pSPIx->SPI_DR = *((uint16_t*)pTxBuffer);
			// TYPE CAST POINTER TO WRITE 16BIT DATA
			pTxBuffer+=2;// MOVE TO NEXT FRAME
			Tx_len-=2;
		}
		else// 8BIT FRAME
		{
			pSPIx->SPI_DR = *pTxBuffer;
			pTxBuffer++;
			Tx_len--;
		}
	                                                                         /* READ DATA REGISTER TO CLEAR RXNE FLAG : PREVENTING OVERRUN ERROR */
		//while(!(pSPIx->SPI_SR&(1<<SPI_SR_RXNE)));
		//(void)pSPIx->SPI_DR;// Dummy read
	}
	                                                                         /* WAIT TILL COMMUNICATION IS OVER (BUSY FLAG = LOW)                */
	    while((pSPIx->SPI_SR&(1<<SPI_SR_BSY)));
}
/*************************************************************************
 * @fn                - SPI_Recieve
 *
 * @brief             - This function receives data from Slave
 *
 * @param[in]         - pSPIx
 * @param[in]         - pRxBuffer : Pointer to receive buffer
 * @param[in]         - Rx_len    : Length of data to be received (number of bytes)
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_Recieve(SPI_Reg_t *pSPIx, uint8_t *pRxBuffer, uint8_t Rx_len)
{
	while(Rx_len>0)
	{
		                                                                        /* SEND DUMMY DATA TO GENERATE CLOCK       */
		//while(!(pSPIx->SPI_SR&(1<<SPI_SR_TXE)));
		//pSPIx->SPI_DR = 0xFF;//WILL AUTO-SEND 0X00FF IF DFF=1

		                                                                        /* WAIT TILL DATA RECEIVED (RXNE is set)   */
	    while(!(pSPIx->SPI_SR&(1<<SPI_SR_RXNE)));

	                                                                            /* READ DATA REGISTER ( BASED ON DFF )     */
	    if(pSPIx->SPI_CR1 & (1<<SPI_CR1_DFF))// 16 BIT FRAME
	    {
	    	*((uint16_t*)pRxBuffer) = pSPIx->SPI_DR;
	    	// TYPE CAST POINTER TO STORE 16BIT DATA

	    	pRxBuffer+=2;
	    	Rx_len-=2;
	    }
	    else// 8BIT FRAME
	    {
	    	*pRxBuffer= pSPIx->SPI_DR;// READ DATA
	    	pRxBuffer++;
	    	Rx_len--;
	    }


	}
	                                                                            /* WAIT TILL COMMUNICATION IS OVER (BUSY FLAG = LOW) */
		 while((pSPIx->SPI_SR&(1<<SPI_SR_BSY)));
}
/*************************************************************************
 * @fn                - SPI_ClearOVRFlag
 *
 * @brief             - This function Clears the Overrun Flag
 *
 * @param[in]         - pSPIx
 *
 * @return            - none
 *
 * @Note              - none
 */
void SPI_ClearOVRFlag(SPI_Reg_t* pSPIx)
{
	uint32_t temp;
	    /* READ DATA REGISTER FIRST : CLEARS RXNE*/
		/* READ STATUS REGISTER NEXT: CLEARS OVR */
	temp= pSPIx->SPI_DR;
	temp= pSPIx->SPI_SR;
	(void)temp;

}





