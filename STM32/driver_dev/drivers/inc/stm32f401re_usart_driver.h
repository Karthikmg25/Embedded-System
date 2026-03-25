/*
 * stm32f401re_usart_driver.h
 *
 *  Created on: Mar 3, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_USART_DRIVER_H_
#define INC_STM32F401RE_USART_DRIVER_H_

#include "STM32F401RE.h"

/*
 * Configuration structure for USART Instance
 *
 */
typedef struct
{
	uint8_t USART_Mode;          // RX,TX,RX and TX
	uint32_t USART_Baud;
	uint8_t USART_WordLength;    // 8bits, 9bits
	uint8_t USART_StopBit;       // 1, 0.5, 2, 1.5
	uint8_t USART_Parity;        // none, odd, even
	uint8_t USART_OverSampling;  // 16 times or 8 times
	uint8_t USART_HWflowControl; // none, CTS, RTS, CTS+RTS

}USART_Config_t;

/*
 * Handle structure for USART Instance
 *
 */
typedef struct
{
	USART_Reg_t *pUSARTx;

	USART_Config_t USART_Config;

}USART_Handle_t;


/*
 * APIs for USART
 *
 */
void USART_PeriClockControl(USART_Reg_t *pUSARTx, uint8_t EnorDi);                    /* ENABLE CLOCK */
void USART_PeripheralControl(USART_Reg_t *pUSARTx, uint8_t EnorDi);                   /* ENBLE OR DISABLE PERIPHERAL*/

void USART_Set_BaudRate(USART_Reg_t *pUSARTx, uint32_t BaudRate);                     /* SETS BAUD RATE */
void USART_Init(USART_Handle_t *pUSART_Handle);                                       /* SETS CONFIGURATIONS */
void USART_Transmit(USART_Reg_t *pUSARTx, uint16_t byte);                             /* TRANSMITS 8 BIT/9 BIT */
uint16_t USART_Recieve(USART_Reg_t *pUSARTx);                                         /* RECEIVES  8 BIT/9 BIT*/
void USART_Transmit_Buffer(USART_Reg_t *pUSARTx, uint8_t* Tx_Buffer, uint8_t Tx_Len); /* TRANSMITS A BUFFER*/
void USART_Receive_Buffer(USART_Reg_t *pUSARTx, uint8_t* Rx_Buffer, uint8_t Rx_Len);  /* RECEIVES A BUFFER*/
void USART_Transmit_String(USART_Reg_t *pUSARTx, char* str);                          /* TRANSMITS A STRING*/
void USART_Receive_String(USART_Reg_t *pUSARTx, char* str, uint8_t buff_size);
/*
 * @USART Modes
 */
#define USART_MODE_RX_ONLY     0
#define USART_MODE_TX_ONLY     1
#define USART_MODE_RXTX        2

/*
 * @USART Standard Baud Rates
 */

#define USART_STD_BAUD_1200     1200
#define USART_STD_BAUD_2400     2400
#define USART_STD_BAUD_9600     9600
#define USART_STD_BAUD_19200    19200
#define USART_STD_BAUD_38400    38400
#define USART_STD_BAUD_57600    57600
#define USART_STD_BAUD_115200   115200
#define USART_STD_BAUD_230400   230400
#define USART_STD_BAUD_460800   460800
#define USART_STD_BAUD_921600   921600

/*
 * @USART Word Length
 */

#define USART_WORDLEN_8BITS    0
#define USART_WORDLEN_9BITS    1

/*
 * @USART Stop Bits
 */

#define USART_STOP_BIT_1         0
#define USART_STOP_BIT_0_5       1
#define USART_STOP_BIT_2         2
#define USART_STOP_BIT_1_5       3
/*
 * @USART Parity
 */

#define  USART_NO_PARITY       0
#define  USART_EVEN_PARITY     1
#define  USART_ODD_PARITY      2
/*
 * @USART OverSampling
 */
#define USART_OVERSAMPLING_16   0
#define USART_OVERSAMPLING_8    1
/*
 * @USART Hard ware Flow Control
 */
#define USART_NO_CTS_RTS   0
#define USART_CTS          1
#define USART_RTS          2
#define USART_CTS_RTS      3
/*
 * @USART Control Bits
 */
#define USART_CR1_OVER8    15  /* OVERSAMPLING           */
#define USART_CR1_UE       13  /* USART ENABLE           */
#define USART_CR1_M        12  /* WORD LENGTH            */
#define USART_CR1_PCE      10  /* PARITY CONTROL ENABLE  */
#define USART_CR1_PS       9   /* PARITY SELECTION       */
#define USART_CR1_TE       3   /* TRANSMIT ENABLE        */
#define USART_CR1_RE       2   /* RECEIVE ENABLE         */
#define USART_CR2_STOP     12  /* STOP BITS              */
#define USART_CR3_CTSE     9   /* CTS ENABLE             */
#define USART_CR3_RTSE     8   /* RTS ENABLE             */

/*
 * @USART Flags
 */
#define USART_SR_TXE   (1<<7) /* TRANSMIT BUFFER FREE                         */
#define USART_SR_TC    (1<<6) /* END OF TRANSMISSION                          */
#define USART_SR_RXNE  (1<<5) /* DATA RECEIVED IN BUFFER                      */
#define USART_SR_IDLE  (1<<4) /* RX LINE INACTIVE                             */
#define USART_SR_ORE   (1<<3) /* OVERRUN ERROR : NEW DATA OVERRWITES OLD DATA */
#define USART_SR_NF    (1<<2) /* NOICE ERROR DETECTION                        */
#define USART_SR_FE    (1<<1) /* FRAMING ERROR DETECTION                      */
#define USART_SR_PE    (1<<0) /* PARITY ERROR DETECTION                       */


#endif /* INC_STM32F401RE_USART_DRIVER_H_ */
