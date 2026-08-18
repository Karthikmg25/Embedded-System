/*
 * STM32F401xx.h
 *
 *  Created on: Jul 24, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401XX_H_
#define INC_STM32F401XX_H_

#include "stdint.h"

#define __vo 					volatile

/*
 * Memory Base Addresses
 */
#define FLASH_BASEADDR			0x08000000U
#define ROM_BASEADDR			0x1FFF0000U
#define SRAM_BASEADDR			0x20000000U

/* NVIC Register Definitions */
// Interrupt Set Enable Registers (Enable interrupts)
#define NVIC_ISER0    			((__vo uint32_t*)0xE000E100)
#define NVIC_ISER1   			((__vo uint32_t*)0xE000E104)
#define NVIC_ISER2  			((__vo uint32_t*)0xE000E108)

// Interrupt Clear Enable Registers (Disable interrupts)
#define NVIC_ICER0   			((__vo uint32_t*)0xE000E180)
#define NVIC_ICER1    			((__vo uint32_t*)0xE000E184)
#define NVIC_ICER2    			((__vo uint32_t*)0xE000E188)

// Interrupt Priority Registers
#define NVIC_PR_BASE_ADDR  		((__vo uint32_t*)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED	4

/*
 * Peripheral Bus Addresses
 */
#define PERIPH_BASEADDR 		0x40000000U
#define APB1_BASEADDR 		    PERIPH_BASEADDR
#define APB2_BASEADDR 		    0x40010000U
#define AHB1_BASEADDR 		    0x40020000U
#define AHB2_BASEADDR 		    0x50000000U

/*
 * AHB1 Peripheral Base Addresses
 * GPIO Ports
 */
#define GPIOA_BASEADDR			(AHB1_BASEADDR + 0x0000)
#define GPIOB_BASEADDR			(AHB1_BASEADDR + 0x0400)
#define GPIOC_BASEADDR			(AHB1_BASEADDR + 0x0800)
#define GPIOD_BASEADDR			(AHB1_BASEADDR + 0x0C00)
#define GPIOE_BASEADDR			(AHB1_BASEADDR + 0x1000)
#define GPIOH_BASEADDR			(AHB1_BASEADDR + 0x1C00)

/*
 * Other AHB1 Peripherals
 */
#define CRC_BASEADDR 			(AHB1_BASEADDR + 0x3000)
#define RCC_BASEADDR 			(AHB1_BASEADDR + 0x3800)
#define FLASH_IF_BASEADDR 		(AHB1_BASEADDR + 0x3C00)
#define DMA1_BASEADDR 			(AHB1_BASEADDR + 0x6000)
#define DMA2_BASEADDR 			(AHB1_BASEADDR + 0x400)

/*
 * APB1 Peripheral Base Addresses
 * Timers
 */
#define TIM2_BASEADDR			(APB1_BASEADDR + 0x0000)
#define TIM3_BASEADDR			(APB1_BASEADDR + 0x0400)
#define TIM4_BASEADDR			(APB1_BASEADDR + 0x0800)
#define TIM5_BASEADDR			(APB1_BASEADDR + 0x0C00)

/*
 * SPI
 */
#define SPI2_BASEADDR			(APB1_BASEADDR + 0x3800)
#define SPI3_BASEADDR			(APB1_BASEADDR + 0x3C00)

/*
 * USART
 */
#define USART2_BASEADDR			(APB1_BASEADDR + 0x4400)

/*
 * I2C
 */
#define I2C1_BASEADDR       	(APB1_BASEADDR + 0x5400)
#define I2C2_BASEADDR      		(APB1_BASEADDR + 0x5800)
#define I2C3_BASEADDR      	    (APB1_BASEADDR + 0x5C00)

/*
 * APB2 Peripheral Base Addresses
 * Timers
 */
#define TIM1_BASEADDR			(APB2_BASEADDR + 0x0000)
#define TIM9_BASEADDR			(APB2_BASEADDR + 0x4000)
#define TIM10_BASEADDR			(APB2_BASEADDR + 0x4400)
#define TIM11_BASEADDR			(APB2_BASEADDR + 0x4800)

/*
 * USART
 */
#define USART1_BASEADDR			(APB2_BASEADDR + 0x1000)
#define USART6_BASEADDR			(APB2_BASEADDR + 0x1400)

/*
 * SPI
 */
#define SPI1_BASEADDR			(APB2_BASEADDR + 0x3000)
#define SPI4_BASEADDR			(APB2_BASEADDR + 0x3400)

/* IRQ Number Definitions */
#define IRQ_NO_EXTI0        6      // EXTI Line 0
#define IRQ_NO_EXTI1        7      // EXTI Line 1
#define IRQ_NO_EXTI2        8      // EXTI Line 2
#define IRQ_NO_EXTI3        9      // EXTI Line 3
#define IRQ_NO_EXTI4        10     // EXTI Line 4
#define IRQ_NO_EXTI9_5      23     // EXTI Lines 5-9 (shared)
#define IRQ_NO_EXTI15_10    40     // EXTI Lines 10-15 (shared)

/*
 * System Configuration
 */
#define SYSCFG_BASEADDR     	(APB2_BASEADDR + 0x3800)
#define EXTI_BASEADDR     		(APB2_BASEADDR + 0x3C00)

/* Peripheral Reset Macros */
/* GPIO Reset - using do-while(0) for safe macro usage */
#define GPIOA_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 0)); (RCC->AHB1RSTR &= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 1)); (RCC->AHB1RSTR &= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 2)); (RCC->AHB1RSTR &= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 3)); (RCC->AHB1RSTR &= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 4)); (RCC->AHB1RSTR &= ~(1 << 4));}while(0)
#define GPIOH_REG_RESET() 		do{(RCC->AHB1RSTR |= (1 << 7)); (RCC->AHB1RSTR &= ~(1 << 7));}while(0)

/* SPI Reset */
#define SPI1_REG_RESET()  		do{(RCC->APB2RSTR |= (1 << 12)); (RCC->APB2RSTR &= ~(1 << 12));}while(0)
#define SPI2_REG_RESET()  		do{(RCC->APB1RSTR |= (1 << 14)); (RCC->APB2RSTR &= ~(1 << 14));}while(0)
#define SPI3_REG_RESET()  		do{(RCC->APB1RSTR |= (1 << 15)); (RCC->APB2RSTR &= ~(1 << 15));}while(0)
#define SPI4_REG_RESET()  		do{(RCC->APB2RSTR |= (1 << 13)); (RCC->APB2RSTR &= ~(1 << 13));}while(0)

/*
 * GPIO Register Definition Structure
 */
typedef struct
{
	__vo uint32_t	MODER;
	__vo uint32_t	OTYPER;
	__vo uint32_t	OSPEEDR;
	__vo uint32_t	PUPDR;
	__vo uint32_t	IDR;
	__vo uint32_t	ODR;
	__vo uint32_t	BSRR;
	__vo uint32_t	LCKR;
	__vo uint32_t	AFR[2];

}GPIO_RegDef_t;

/*
 * Peripheral register definition structure for RCC
 */
typedef struct
{
    __vo uint32_t	CR;
    __vo uint32_t	PLLCFGR;
    __vo uint32_t	CFGR;
    __vo uint32_t	CIR;
    __vo uint32_t	AHB1RSTR;
    __vo uint32_t	AHB2RSTR;
         uint32_t	Reserved[14];
    __vo uint32_t	APB1RSTR;
    __vo uint32_t	APB2RSTR;
    __vo uint32_t	AHB1ENR;
    __vo uint32_t	AHB2ENR;
    __vo uint32_t	APB1ENR;
    __vo uint32_t	APB2ENR;
    __vo uint32_t	AHB1LPENR;
    __vo uint32_t	AHB2LPENR;
    __vo uint32_t	APB1LPENR;
    __vo uint32_t	APB2LPENR;
    __vo uint32_t	BDCR;
    __vo uint32_t	CSR;
    __vo uint32_t	SSCGR;
    __vo uint32_t	PLLI2SCFGR;
    __vo uint32_t	DCKCFGR;
}RCC_RegDef_t;

/*
 * Peripheral register definition structure for SPI
 */
typedef struct
{
	__vo uint32_t	CR1;
	__vo uint32_t	SR;
	__vo uint32_t	DR;
	__vo uint32_t	CRCPR;
	__vo uint32_t	RXCRCR;
	__vo uint32_t	TXCRCR;
	__vo uint32_t	I2SCFGR;
	__vo uint32_t	I2SPR;
}SPI_RegDef_t;

/*
 * Peripheral register definition structure for I2C
 */
typedef struct
{
	__vo uint32_t	CR1;
	__vo uint32_t	CR2;
	__vo uint32_t	OAR1;
	__vo uint32_t	OAR2;
	__vo uint32_t	DR;
	__vo uint32_t	SR1;
	__vo uint32_t	SR2;
	__vo uint32_t	CCR;
	__vo uint32_t	TRISE;
	__vo uint32_t	FLTR;
}I2C_RegDef_t;

/*
 * Peripheral register definition structure for USART
 */
typedef struct
{
	__vo uint32_t	SR;
	__vo uint32_t	DR;
	__vo uint32_t	BRR;
	__vo uint32_t	CR1;
	__vo uint32_t	CR2;
	__vo uint32_t	CR3;
	__vo uint32_t	GTPR;
}USART_RegDef_t;

/*
 * Peripheral register definition of EXTI
 */
typedef struct
{
    __vo uint32_t IMR;     // Interrupt Mask Register         - Offset: 0x00
    __vo uint32_t EMR;     // Event Mask Register             - Offset: 0x04
    __vo uint32_t RTSR;    // Rising Trigger Selection Reg    - Offset: 0x08
    __vo uint32_t FTSR;    // Falling Trigger Selection Reg   - Offset: 0x0C
    __vo uint32_t SWIER;   // Software Interrupt Event Reg    - Offset: 0x10
    __vo uint32_t PR;      // Pending Register                - Offset: 0x14
} EXTI_RegDef_t;

/*
 * SYSCFG Definition
 */
typedef struct
{
    __vo uint32_t MEMRMP;           // Memory Remap Register       - Offset: 0x00
    __vo uint32_t PMC;              // Peripheral Mode Config      - Offset: 0x04
    __vo uint32_t EXTICR[4];        // External Interrupt Config   - Offset: 0x08-0x14
    	 uint32_t RESERVED1[2];     // Reserved                    - Offset: 0x18-0x1C
    __vo uint32_t CMPCR;            // Compensation Cell Control   - Offset: 0x20
} SYSCFG_RegDef_t;

/**********************************
 * Peripheral Definitions
 * (Peripheral base addresses type casted to xxx_RegDef_t)
 **********************************/

/* GPIO */
#define GPIOA				((GPIO_RegDef_t *)GPIOA_BASEADDR)
#define GPIOB				((GPIO_RegDef_t *)GPIOB_BASEADDR)
#define GPIOC				((GPIO_RegDef_t *)GPIOC_BASEADDR)
#define GPIOD				((GPIO_RegDef_t *)GPIOD_BASEADDR)
#define GPIOE				((GPIO_RegDef_t *)GPIOE_BASEADDR)
#define GPIOH				((GPIO_RegDef_t *)GPIOH_BASEADDR)

/* RCC */
#define RCC					((RCC_RegDef_t *)RCC_BASEADDR)

/* SPI */
#define SPI1				((SPI_RegDef_t *)SPI1_BASEADDR)
#define SPI2				((SPI_RegDef_t *)SPI2_BASEADDR)
#define SPI3				((SPI_RegDef_t *)SPI3_BASEADDR)
#define SPI4				((SPI_RegDef_t *)SPI4_BASEADDR)

/* I2C */
#define I2C1				((I2C_RegDef_t *)I2C1_BASEADDR)
#define I2C2				((I2C_RegDef_t *)I2C2_BASEADDR)
#define I2C3				((I2C_RegDef_t *)I2C3_BASEADDR)

/* USART */
#define USART1				((USART_RegDef_t *)USART1_BASEADDR)
#define USART2				((USART_RegDef_t *)USART2_BASEADDR)
#define USART6				((USART_RegDef_t *)USART6_BASEADDR)

#define EXTI     		    ((EXTI_RegDef_t*)EXTI_BASEADDR)
#define SYSCFG       	    ((SYSCFG_RegDef_t*)SYSCFG_BASEADDR)

/**********************************
 * Clock Enable Macros for GPIOx
  **********************************/
/* GPIOx clocks are on AHB1 bus */
#define GPIOA_PCLK_EN()		(RCC->AHB1ENR |= (1<<0))
#define GPIOB_PCLK_EN()		(RCC->AHB1ENR |= (1<<1))
#define GPIOC_PCLK_EN()		(RCC->AHB1ENR |= (1<<2))
#define GPIOD_PCLK_EN()		(RCC->AHB1ENR |= (1<<3))
#define GPIOE_PCLK_EN()		(RCC->AHB1ENR |= (1<<4))
#define GPIOH_PCLK_EN()		(RCC->AHB1ENR |= (1<<7))

/* SPI Clock Enable */
#define SPI1_PCLK_EN()		(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()		(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()		(RCC->APB1ENR |= (1<<15))
#define SPI4_PCLK_EN()		(RCC->APB2ENR |= (1<<13))

/* I2C Clock Enable */
#define I2C1_PCLK_EN()		(RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()		(RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()		(RCC->APB1ENR |= (1<<23))

/* USART Clock Enable */
#define USART1_PCLK_EN()	(RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()	(RCC->APB1ENR |= (1<<17))
#define USART6_PCLK_EN()	(RCC->APB2ENR |= (1<<5))

/**********************************
 * Clock Disable Macros
 **********************************/
/* GPIO Clock Disable */
#define GPIOA_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<4))
#define GPIOH_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<7))

/* SPI Clock Disable */
#define SPI1_PCLK_DI()		(RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()		(RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()		(RCC->APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DI()		(RCC->APB2ENR &= ~(1<<13))

#define SYSCFG_PCLK_EN()	(RCC->APB2ENR |= (1<<14))

/* I2C Clock Disable */
#define I2C1_PCLK_DI()		(RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI()		(RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI()		(RCC->APB1ENR &= ~(1<<23))

/* USART Clock Disable */
#define USART1_PCLK_DI()	(RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI()	(RCC->APB1ENR &= ~(1<<17))
#define USART6_PCLK_DI()	(RCC->APB2ENR &= ~(1<<5))

/* General Purpose Macros */
#define ENABLE				1
#define DISABLE				0
#define SET					ENABLE
#define RESET				DISABLE
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET  	RESET
#define FLAG_RESET			DISABLE
#define FLAG_SET			SET

#define GPIO_BASEADDR_TO_CODE(x) \
( (x) == GPIOA ? 0 : \
  (x) == GPIOB ? 1 : \
  (x) == GPIOC ? 2 : \
  (x) == GPIOD ? 3 : \
  (x) == GPIOE ? 4 : \
  (x) == GPIOH ? 7 : 0 )

/* Bit Position Definitions - SPI_CR1 */
#define SPI_CR1_CPHA        0   // Clock Phase
#define SPI_CR1_CPOL        1   // Clock Polarity
#define SPI_CR1_MSTR        2   // Master Selection
#define SPI_CR1_BR          3   // Baud Rate Control (3 bits)
#define SPI_CR1_SPE         6   // SPI Enable
#define SPI_CR1_LSBFIRST    7   // LSB First
#define SPI_CR1_SSI         8   // Internal Slave Select
#define SPI_CR1_SSM         9   // Software Slave Management
#define SPI_CR1_RXONLY      10  // Receive Only
#define SPI_CR1_DFF         11  // Data Frame Format
#define SPI_CR1_CRCNEXT     12	// CRC transfer next
#define SPI_CR1_CRCEN       13	// Hardware CRC calculation enable
#define SPI_CR1_BIDIOE		14	// Bidirectional Output Enable
#define SPI_CR1_BIDIMODE    15  // Bidirectional Mode

/* Bit Position Definitions - SPI_CR2 */
#define SPI_CR2_RXDMAEN     0   // Rx buffer DMA enable
#define SPI_CR2_TXDMAEN     1   // Tx buffer DMA enable
#define SPI_CR2_SSOE        2   // SS output enable
#define SPI_CR2_FRF         4   // Frame format
#define SPI_CR2_ERRIE       5   // Error interrupt enable
#define SPI_CR2_RXNEIE      6   // RX buffer not empty interrupt enable
#define SPI_CR2_TXEIE       7   // Tx buffer empty interrupt enable

/* Bit Position Definitions - SPI_SR */
#define SPI_SR_RXNE         0   // RX Not Empty
#define SPI_SR_TXE          1   // TX Empty
#define SPI_SR_CHSIDE       2   // Channel Side
#define SPI_SR_UDR          3   // Underrun
#define SPI_SR_CRCERR       4   // CRC Error
#define SPI_SR_MODF         5   // Mode Fault
#define SPI_SR_OVR          6   // Overrun
#define SPI_SR_BSY          7   // Busy
#define SPI_SR_FRE          8   // Frame format error


#endif /* INC_STM32F401XX_H_ */
