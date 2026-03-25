/*
 * stm32f401re_usart_driver.c
 *
 *  Created on: Mar 3, 2026
 *      Author: hp
 */
#include "stm32f401re_usart_driver.h"
/*************************************************************************
 * @fn                - USART_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given USART Peripheral
 *
 * @param[in]         - pUSARTx: base address of the USART peripheral(USART1, USART2, USART6)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - none
 *
 * @Note              - none
 */
void USART_PeriClockControl(USART_Reg_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
		{
		        if(pUSARTx== USART1)
	              {
		        	USART1_CLK_EN();
	              }
		  else  if(pUSARTx== USART2)
		          {
			        USART2_CLK_EN();
		          }

		  else  if(pUSARTx== USART6)
		          {
			         USART6_CLK_EN();
		          }

		}
		else
		{
			          if(pUSARTx== USART1)
			              {
			        	      USART1_CLK_DI();
			              }
				  else  if(pUSARTx== USART2)
				          {
					          USART2_CLK_DI();
				          }

				  else  if(pUSARTx== USART6)
				          {
					         USART6_CLK_DI();
				          }

		}
}
/*************************************************************************
 * @fn                - USART_PerpheralControl
 *
 * @brief             - This function Enables or Disables a USART Peripheral
 *
 * @param[in]         - pUSARTx
 * @param[in]         - EnorDi
 *
 * @return            - none
 *
 * @Note              - none
 */
void USART_PeripheralControl(USART_Reg_t *pUSARTx, uint8_t EnorDi)
{
	if(EnorDi== ENABLE)
	{
		pUSARTx->USART_CR1 |= (1<<USART_CR1_UE);
	}
	else
	{
		pUSARTx->USART_CR1 &=~ (1<<USART_CR1_UE);
	}

}
/*************************************************************************
 * @fn                - USART_Set_BaudRate
 *
 * @brief             - This function Sets the required baud rate
 *
 * @param[in]         - pUSARTx
 * @param[in]         - BaudRate
 *
 * @return            - none
 *
 * @Note              - none
 */
void USART_Set_BaudRate(USART_Reg_t *pUSARTx, uint32_t BaudRate)
{
	uint32_t int_part, fra_part, usartdiv;
	uint32_t pCLK= 16000000;
	if(pUSARTx->USART_CR1&(1<<USART_CR1_OVER8))  // over-sampling 8 times
	{
		  usartdiv= (100*pCLK)/(8*BaudRate);

		  int_part= usartdiv/100; // mantissa part

    	  fra_part= usartdiv- (int_part*100);// fraction part

		  fra_part= ((fra_part*8)+50)/100;// over-sampling 8 times
					                                  // add 50 to avoid loosing precision
		  pUSARTx->USART_BRR = (int_part<<4) | (fra_part & 0x7);
	}

	else// over-sampling 16 times
	{
		    usartdiv= (100*pCLK)/(16*BaudRate);

			int_part= usartdiv/100; // mantissa part

			fra_part= usartdiv- (int_part*100);// fraction part

			fra_part= ((fra_part*16)+50)/100;// over-sampling 16 times
			                                  // add 50 to avoid loosing precision
			pUSARTx->USART_BRR = (int_part<<4) | (fra_part & 0xF);
	}

}

/*************************************************************************
 * @fn                - USART_Init
 *
 * @brief             - This function initializes UART with required configurations
 *
 * @param[in]         - pUSARTx_Handle
 *
 * @return            - none
 *
 * @Note              - none
 */
void USART_Init(USART_Handle_t *pUSART_Handle)
{
	USART_PeriClockControl(pUSART_Handle->pUSARTx, ENABLE);                                                 /* ENABLE CLOCK FOR THE PERIPHERAL */

                                                                                                            /* SET OVERSAMPLING BEFORE SETTING BAUDRATE*/
    pUSART_Handle->pUSARTx->USART_CR1 &=~(1<<USART_CR1_OVER8);//clear bit
    pUSART_Handle->pUSARTx->USART_CR1 |=(pUSART_Handle->USART_Config.USART_OverSampling<< USART_CR1_OVER8);

	USART_Set_BaudRate(pUSART_Handle->pUSARTx, pUSART_Handle->USART_Config.USART_Baud);                     /* SET BAUD RATE */

	                                                                                                        /* SET MODE*/
	if(pUSART_Handle->USART_Config.USART_Mode==USART_MODE_RX_ONLY)
	{
		pUSART_Handle->pUSARTx->USART_CR1 |= (1<<USART_CR1_RE);// RX Enable
		pUSART_Handle->pUSARTx->USART_CR1 &=~ (1<<USART_CR1_TE);// TX Disable
	}
	else if(pUSART_Handle->USART_Config.USART_Mode==USART_MODE_TX_ONLY)
	{
		pUSART_Handle->pUSARTx->USART_CR1 &=~ (1<<USART_CR1_RE);// RX Disable
		pUSART_Handle->pUSARTx->USART_CR1 |= (1<<USART_CR1_TE); // TX Enable
	}
	else if(pUSART_Handle->USART_Config.USART_Mode==USART_MODE_RXTX)
	{
		pUSART_Handle->pUSARTx->USART_CR1 |= (1<<USART_CR1_RE); // RX Enable
		pUSART_Handle->pUSARTx->USART_CR1 |= (1<<USART_CR1_TE); // TX Enable
	}
	                                                                                                          /* SET WORD LENGTH */
	pUSART_Handle->pUSARTx->USART_CR1 &=~(1<<USART_CR1_M ); // clear bit
	pUSART_Handle->pUSARTx->USART_CR1 |= (pUSART_Handle->USART_Config.USART_WordLength << USART_CR1_M );

	                                                                                                          /* SET PARITY */
	if(pUSART_Handle->USART_Config.USART_Parity== USART_NO_PARITY)
	{
		pUSART_Handle->pUSARTx->USART_CR1 &=~(1<<USART_CR1_PCE);// disable parity
	}
	else if(pUSART_Handle->USART_Config.USART_Parity== USART_EVEN_PARITY)
	{
		pUSART_Handle->pUSARTx->USART_CR1 |=(1<<USART_CR1_PCE);// enable parity
		pUSART_Handle->pUSARTx->USART_CR1 &=~(1<<USART_CR1_PS);// even parity
	}
	else if(pUSART_Handle->USART_Config.USART_Parity== USART_ODD_PARITY)
	{
		pUSART_Handle->pUSARTx->USART_CR1 |=(1<<USART_CR1_PCE);// enable parity
		pUSART_Handle->pUSARTx->USART_CR1 |= (1<<USART_CR1_PS);// odd parity
	}
                                                                                                            /* SET STOP BITS */
	pUSART_Handle->pUSARTx->USART_CR2 &=~ (0x3<<USART_CR2_STOP);//clear bits
	pUSART_Handle->pUSARTx->USART_CR2 |= (pUSART_Handle->USART_Config.USART_StopBit<<USART_CR2_STOP);


	                                                                                                        /* SET HARDWARE FLOW CONTROL*/
	 if(pUSART_Handle->USART_Config.USART_HWflowControl==USART_NO_CTS_RTS)
	 {
		 pUSART_Handle->pUSARTx->USART_CR3 &=~((1<<USART_CR3_CTSE )|(1<<USART_CR3_RTSE));//disable both
	 }
	 else if(pUSART_Handle->USART_Config.USART_HWflowControl==USART_CTS)
	 {
	     pUSART_Handle->pUSARTx->USART_CR3 |= (1<<USART_CR3_CTSE);//enable CTS
		 pUSART_Handle->pUSARTx->USART_CR3 &=~(1<<USART_CR3_RTSE);// disable RTS
	 }
	 else if(pUSART_Handle->USART_Config.USART_HWflowControl==USART_RTS)
	 {
	     pUSART_Handle->pUSARTx->USART_CR3 |= (1<<USART_CR3_RTSE);// enable RTS
		 pUSART_Handle->pUSARTx->USART_CR3 &=~(1<<USART_CR3_CTSE);// disable CTS
	 }
	 else if(pUSART_Handle->USART_Config.USART_HWflowControl==USART_CTS_RTS)
     {
	     pUSART_Handle->pUSARTx->USART_CR3 |= (1<<USART_CR3_RTSE);// enable RTS
	     pUSART_Handle->pUSARTx->USART_CR3 |= (1<<USART_CR3_CTSE);// enable CTS
	  }

}

/*************************************************************************
 * @fn                - USART_Transmit
 *
 * @brief             - This function transmits a single USART data frame
 *
 * @param[in]         - pUSARTx
 * @param[in]         - byte
 *
 * @return            - none
 *
 * @Note              - This API sends frame based on the configured word length. Parity handling is not implemented
 */
void USART_Transmit(USART_Reg_t *pUSARTx, uint16_t byte)
{
	while(!(pUSARTx->USART_SR & USART_SR_TXE));// wait till transmit buffer is empty

	if(pUSARTx->USART_CR1&(1<<USART_CR1_M))// 9 bit word length
	{
		pUSARTx->USART_DR= byte&0x1FF;                   // write data for transmission
	}
	else// 8 bit word length
	{
		pUSARTx->USART_DR= byte&0xFF;                   // write data for transmission
	}
}
/*************************************************************************
 * @fn                - USART_Receive
 *
 * @brief             - This function Receives a single USART data frame
 *
 * @param[in]         - pUSARTx
 *
 * @return            - Received Frame
 *
 * @Note              - This API returns the received frame based on the configured word length. Parity handling is not implemented.
 */
uint16_t USART_Recieve(USART_Reg_t *pUSARTx)
{
	while(!(pUSARTx->USART_SR & USART_SR_RXNE)); // wait till data received in buffer

	if(pUSARTx->USART_CR1&(1<<USART_CR1_M))// 9 bit word length
	{
		return (pUSARTx->USART_DR)&0x1FF;//return 9bits
	}
	else// 8 bit word length
	{
		return (pUSARTx->USART_DR)&0xFF;//return 8bits
	}

}
/*************************************************************************
 * @fn                - USART_Transmit_Buffer
 *
 * @brief             - This function Transmits a buffer from the device
 *
 * @param[in]         - pUSARTx
 * @param[in]         - Tx_Buffer
 * @param[in]         - Tx_Len : no of bytes
 *
 * @return            - none
 *
 * @Note              - Transmits data based on word length and parity
 */

void USART_Transmit_Buffer(USART_Reg_t *pUSARTx, uint8_t* Tx_Buffer, uint8_t Tx_Len)
{
	while(Tx_Len>0)
	{
		while(!(pUSARTx->USART_SR & USART_SR_TXE));// wait till transmit buffer is empty

		if(pUSARTx->USART_CR1&(1<<USART_CR1_M)) /*9 BIT WORD LENGTH*/
		{
			if(pUSARTx->USART_CR1&(1<<USART_CR1_PCE))// parity enabled : 1 bit used for parity, transmits 8 data bits
			{
				pUSARTx->USART_DR= *Tx_Buffer;//8 bits
				Tx_Buffer++;
				Tx_Len--;
			}
			else                                // parity disabled : no parity bits, transmits full 9 data bits
			{
				pUSARTx->USART_DR= (*((uint16_t*)Tx_Buffer) & 0x1FF);// 9 bits
				Tx_Buffer+=2;
				Tx_Len-=2;
			}
		}
		else                                 /*8 BIT WORD LENGTH*/
		{
			if(pUSARTx->USART_CR1&(1<<USART_CR1_PCE))// parity enabled : 1 bit used for parity, transmits 7 data bits
			{
				pUSARTx->USART_DR= (*Tx_Buffer)& 0X7F;// 7 bits
				Tx_Buffer++;
				Tx_Len--;
			}
			else                                // parity disabled : no parity bits, transmits full 8 data bits
			{
				pUSARTx->USART_DR= *Tx_Buffer;// 8 bits
				Tx_Buffer++;
				Tx_Len--;

			}
		}

	}
	while(!(pUSARTx->USART_SR& USART_SR_TC));// wait till end of transmission

}
/*************************************************************************
 * @fn                - USART_Receive_Buffer
 *
 * @brief             - This function Receives a buffer
 *
 * @param[in]         - pUSARTx
 * @param[in]         - Rx_Buffer
 * @param[in]         - Rx_Len : no of bytes
 *
 * @return            - none
 *
 * @Note              - Receives data based on word length and parity
 */

void USART_Receive_Buffer(USART_Reg_t *pUSARTx, uint8_t* Rx_Buffer, uint8_t Rx_Len)
{
	 while(Rx_Len>0)
	 {

	              while(!((pUSARTx->USART_SR) & USART_SR_RXNE))
	            	  ;// wait till data received in buffer

			        if(pUSARTx->USART_CR1&(1<<USART_CR1_M)) /*9 BIT WORD LENGTH*/
					{
						if(pUSARTx->USART_CR1&(1<<USART_CR1_PCE))// parity enabled : 1 bit used for parity, receives 8 data bits
						{
							*Rx_Buffer=(pUSARTx->USART_DR)&0xFF;//8 bits
							Rx_Buffer++;
							Rx_Len--;
						}
						else                                // parity disabled : no parity bits, receives full 9 data bits
						{
							*((uint16_t*)Rx_Buffer)=(pUSARTx->USART_DR)&0x1FF;// 9 bits
							Rx_Buffer+=2;
							Rx_Len-=2;
						}
					}
					else                                 /*8 BIT WORD LENGTH*/
					{
						if(pUSARTx->USART_CR1&(1<<USART_CR1_PCE))// parity enabled : 1 bit used for parity, receives 7 data bits
						{
							*Rx_Buffer=(pUSARTx->USART_DR)&0x7F;//7 bits
							Rx_Buffer++;
							Rx_Len--;
						}
						else                                // parity disabled : no parity bits, receives full 8 data bits
						{
							*Rx_Buffer=(pUSARTx->USART_DR)&0xFF;//8 bits
							Rx_Buffer++;
							Rx_Len--;

						}
					}
	 }
}
/*************************************************************************
 * @fn                - USART_Transmit_String
 *
 * @brief             - This Transmits a String
 *
 * @param[in]         - pUSARTx
 * @param[in]         - str
 *
 * @return            - none
 *
 * @Note              - This API works only for 8 bit mode with no parity
 */
void USART_Transmit_String(USART_Reg_t *pUSARTx, char* str)
{
	while(*str)
	{
		while(!(pUSARTx->USART_SR & USART_SR_TXE));// wait till transmit buffer is empty

		pUSARTx->USART_DR = *str;

		str++;
	}
	while (!(pUSARTx->USART_SR & USART_SR_TC));// wait till transmission ends
}

/*************************************************************************
 * @fn                - USART_Recieve_String
 *
 * @brief             - This Receives a String
 *
 * @param[in]         - pUSARTx
 * @param[in]         - str
 *
 * @return            - none
 *
 * @Note              - This API works only for 8 bit mode with no parity
 */
void USART_Receive_String(USART_Reg_t *pUSARTx, char* str, uint8_t buff_size)
{
	char ch;
	uint8_t i=0;// index
	while(1)
	{
		 while(!((pUSARTx->USART_SR) & USART_SR_RXNE))
			            	  ;// wait till data received in buffer
		ch= pUSARTx->USART_DR;// read received data

		if(ch=='\n')
		{
			break;
		}
		if(i+1 < buff_size)// prevent buffer overflow
		{
			str[i]=ch;// store in buffer
			i++;
		}
		else{break;}

	}
	str[i]='\0';
}









