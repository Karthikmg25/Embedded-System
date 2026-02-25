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
 * @Note              - Refer macros for GPIO configurations
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
		           SP12_CLK_EN();
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
				         SP12_CLK_DI();
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
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_BIBIMODE);       // FULL DUPLEX : MOSI AND MISO ACTIVE
	}
	else
		if(pSPIHandle->SPI_Config.SPI_Bus_Config == SPI_BUS_CONFIG_HD )
			{
				pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_BIBIMODE); // HALF DUPLEX : ONLY MOSI USED AS SINGLE LINE, MISO IGNORED
			}                                                        // DATA DIRACTION DEPENDS ON BIDIOE BIT : INPUT OR OUTPUT
		else
		 if(pSPIHandle->SPI_Config.SPI_Bus_Config == SPI_BUS_CONFIG_SIMPLEX_RX )
			{
			   pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_BIBIMODE);
			   pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_RXONLY); // SIMPLEX : ONLY MISO USED FOR INPUT, M0SI IGNORED
			}

	                                                                                            /* SELECT DATA FRAME FORMAT */
	if(pSPIHandle->SPI_Config.SPI_DataFrame == SPI_DFF_8BIT )
	{
		pSPIHandle->pSPIx->SPI_CR1 &=~(1<<SPI_CR1_DFF);
	}
	else
	{
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_DFF);
	}
	                                                                                           /* SOFTWARE SLAVE MANAGEMENT*/
	if(pSPIHandle->SPI_Config.SPI_SSM == SPI_SSM_EN)
	{
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSM); // SLAVE SELECTED VIA SOFTWARE
		pSPIHandle->pSPIx->SPI_CR1 |= (1<<SPI_CR1_SSI); // MUST BE HIGH IN SSM + MASTER MODE, ELSE MODE FAULT
	}
	else
	{
		pSPIHandle->pSPIx->SPI_CR1 &=~ (1<<SPI_CR1_SSM); // SLAVE SELECTED VIA HARDWARE
	}
	                                                                                            /* SET PRESCALAR*/
	pSPIHandle->pSPIx->SPI_CR1 &=~ (0x3 << SPI_CR1_BR);// CLEAR BITS
	pSPIHandle->pSPIx->SPI_CR1 |= (pSPIHandle->SPI_Config.SPI_CLK_Speed << SPI_CR1_BR);
	                                                                                            /* SPI ENABLE*/

	SPI_PeripheralControl(pSPIHandle->pSPIx, ENABLE);

}












