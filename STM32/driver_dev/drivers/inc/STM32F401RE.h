/*
 * STM32F401RE.h
 *
 *  Created on: Jan 17, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_H_
#define INC_STM32F401RE_H_

/*
 * SRAM   96KB
 * FLASH  512KB
 * ROM    30KB
 */

#define _vo              volatile
#include <stdint.h>

/*
 *  Processor Details
 */
/*
 *  ARM CORTEX M4 Processor NVIC base address
 */
#define NVIC_PERIPH_BASE_ADDR       0XE000E100U
/*
 *   NVIC Register addresses
 */
#define NVIC_ISER0                  ((_vo uint32_t*)0XE000E100)
#define NVIC_ISER1                  ((_vo uint32_t*)0XE000E104)
#define NVIC_ISER2                  ((_vo uint32_t*)0xE000E108)
#define NVIC_ICER0                  ((_vo uint32_t*)0xE000E180)
#define NVIC_ICER1                  ((_vo uint32_t*)0xE000E184)
#define NVIC_ICER2                  ((_vo uint32_t*)0xE000E188)
#define NVIC_PR_BASE ADDR           ((_vo uint32_t*)0xE000E188)


#define NVIC_PERIPH_BASE_ADDR       0XE000E100U
/*
 *  memory base address
 */
#define ROM_BASE_ADDR                0x1FFF0000U
#define SRAM_BASE_ADDR               0x20000000U
#define FLASH_BASE_ADDR              0x08000000U

/*
 *  peripheral bus base address
 */
#define APB1_PERIPH_BASE_ADDR             0x40000000U
#define APB2_PERIPH_BASE_ADDR             0x40010000U
#define AHB1_PERIPH_BASE_ADDR             0x40020000U
#define AHB2_PERIPH_BASE_ADDR             0x50000000U

/*
 *  base address of peripherals in APB1 bus
 */

#define  TIM2_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x0000)
#define  TIM3_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x0400)
#define  TIM4_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x0800)
#define  TIM5_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x0C00)
#define  RTC_BKP_PERIPH_BASE_ADDR               (APB1_PERIPH_BASE_ADDR+ 0x2800)
#define  WWDG_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x2C00)
#define  IWDG_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x3000)
#define  I2S2ext_PERIPH_BASE_ADDR               (APB1_PERIPH_BASE_ADDR+ 0x3400)
#define  SPI2_I2S2_PERIPH_BASE_ADDR             (APB1_PERIPH_BASE_ADDR+ 0x3800)
#define  SPI3_I2S3_PERIPH_BASE_ADDR             (APB1_PERIPH_BASE_ADDR+ 0x3C00)
#define  I2S3ext_PERIPH_BASE_ADDR               (APB1_PERIPH_BASE_ADDR+ 0x4000)
#define  USART2_PERIPH_BASE_ADDR                (APB1_PERIPH_BASE_ADDR+ 0x4400)
#define  I2C1_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x5400)
#define  I2C2_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x5800)
#define  I2C3_PERIPH_BASE_ADDR                  (APB1_PERIPH_BASE_ADDR+ 0x5C00)
#define  PWR_PERIPH_BASE_ADDR                   (APB1_PERIPH_BASE_ADDR+ 0x7000)

/*
 *  base address of peripherals in APB2 bus
 */
#define   TIM1_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x0000)
#define   USART1_PERIPH_BASE_ADDR                      (APB2_PERIPH_BASE_ADDR+ 0x1000)
#define   USART6_PERIPH_BASE_ADDR                      (APB2_PERIPH_BASE_ADDR+ 0x1400)
#define   ADC1_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x2000)
#define   SDIO_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x2C00)
#define   SPI1_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x3000)
#define   SPI4_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x3400)
#define   SYSCFG_PERIPH_BASE_ADDR                      (APB2_PERIPH_BASE_ADDR+ 0x3800)
#define   EXTI_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x3C00)
#define   TIM9_PERIPH_BASE_ADDR                        (APB2_PERIPH_BASE_ADDR+ 0x4000)
#define   TIM10_PERIPH_BASE_ADDR                       (APB2_PERIPH_BASE_ADDR+ 0x4400)
#define   TIM11_PERIPH_BASE_ADDR                       (APB2_PERIPH_BASE_ADDR+ 0x4800)

/*
 *  base address of peripherals in AHB1 bus
 */

#define GPIOA_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x0000)
#define GPIOB_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x0400)
#define GPIOC_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x0800)
#define GPIOD_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x0C00)
#define GPIOE_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x1000)
#define GPIOH_PERIPH_BASE_ADDR           (AHB1_PERIPH_BASE_ADDR+ 0x1C00)
#define CRC_PERIPH_BASE_ADDR             (AHB1_PERIPH_BASE_ADDR+ 0x3000)
#define RCC_PERIPH_BASE_ADDR             (AHB1_PERIPH_BASE_ADDR+ 0x3800)
#define FIR_PERIPH_BASE_ADDR             (AHB1_PERIPH_BASE_ADDR+ 0x3C00)
#define DMA1_PERIPH_BASE_ADDR            (AHB1_PERIPH_BASE_ADDR+ 0x6000)
#define DMA2_PERIPH_BASE_ADDR            (AHB1_PERIPH_BASE_ADDR+ 0x6400)

/*
 *  base address of peripherals in AHB2 bus
 */

#define USB_OTG_FS_PERIPH_BASE_ADDR             (AHB2_PERIPH_BASE_ADDR+ 0x3800)


/*
 *   GPiO register definition
 */
typedef struct
{
  _vo uint32_t MODER;
  _vo uint32_t OTYPER;
  _vo uint32_t OSPEEDR ;
  _vo uint32_t PUPDR;
  _vo uint32_t IDR ;
  _vo uint32_t ODR;
  _vo uint32_t BSRR ;
  _vo uint32_t LCKR;
  _vo uint32_t AFR[2];

}GPIO_Reg_t;
/*
 *   GPIO Pointers
 */
#define GPIOA          ((GPIO_Reg_t*)GPIOA_PERIPH_BASE_ADDR)
#define GPIOB          ((GPIO_Reg_t*)GPIOB_PERIPH_BASE_ADDR)
#define GPIOC          ((GPIO_Reg_t*)GPIOC_PERIPH_BASE_ADDR)
#define GPIOD          ((GPIO_Reg_t*)GPIOD_PERIPH_BASE_ADDR)
#define GPIOE          ((GPIO_Reg_t*)GPIOE_PERIPH_BASE_ADDR)
#define GPIOH          ((GPIO_Reg_t*)GPIOH_PERIPH_BASE_ADDR)


/*
 *   SPI register definition
 */
typedef struct
{
  _vo uint32_t SPI_CR1;
  _vo uint32_t SPI_CR2;
  _vo uint32_t SPI_SR;
  _vo uint32_t SPI_DR;
  _vo uint32_t SPI_CRCPR;
  _vo uint32_t SPI_RXCRCR;
  _vo uint32_t SPI_TXCRCR;
  _vo uint32_t SPI_I2SCFGR;
  _vo uint32_t SPI_I2SPR;

}SPI_Reg_t;
/*
 *   SPI Pointers
 */
#define SPI1               ((SPI_Reg_t*)SPI1_PERIPH_BASE_ADDR)
#define SPI4               ((SPI_Reg_t*)SPI4_PERIPH_BASE_ADDR)
#define SPI2_I2S2          ((SPI_Reg_t*)SPI2_I2S2_PERIPH_BASE_ADDR)
#define SPI3_I2S3          ((SPI_Reg_t*)SPI3_I2S3_PERIPH_BASE_ADDR)


/*
 *   I2C register definition
 */
typedef struct
{
  _vo uint32_t  I2C_CR1;
  _vo uint32_t  I2C_CR2;
  _vo uint32_t  I2C_OAR1;
  _vo uint32_t  I2C_OAR2;
  _vo uint32_t  I2C_DR;
  _vo uint32_t  I2C_SR1;
  _vo uint32_t  I2C_SR2;
  _vo uint32_t  I2C_CCR;
  _vo uint32_t  I2C_TRISE;
  _vo uint32_t  I2C_FLTR;

}I2C_Reg_t;
/*
 *   I2C Pointers
 */
#define I2C1               ((I2C_Reg_t*)I2C1_PERIPH_BASE_ADDR)
#define I2C2               ((I2C_Reg_t*)I2C2_PERIPH_BASE_ADDR)
#define I2C3               ((I2C_Reg_t*)I2C3_PERIPH_BASE_ADDR)


/*
 *   USART register definition
 */
typedef struct
{
  _vo uint32_t   USART_SR;
  _vo uint32_t   USART_DR;
  _vo uint32_t   USART_BRR;
  _vo uint32_t   USART_CR1;
  _vo uint32_t   USART_CR2;
  _vo uint32_t   USART_CR3;
  _vo uint32_t   USART_GTPR;

}USART_Reg_t;
/*
 *   USART Pointers
 */

#define USART1               ((USART_Reg_t*)USART1_PERIPH_BASE_ADDR)
#define USART2               ((USART_Reg_t*)USART2_PERIPH_BASE_ADDR)
#define USART6               ((USART_Reg_t*)USART6_PERIPH_BASE_ADDR)


/*
 *   RCC register definition
 */

typedef struct
{
  _vo uint32_t   CR;
  _vo uint32_t   PLLCFGR;
  _vo uint32_t   RCC_CFGR;
  _vo uint32_t   RCC_CIR;
  _vo uint32_t   AHB1RSTR;
  _vo uint32_t   AHB2RSTR;
  uint32_t       Reserved1[2];
  _vo uint32_t   APB1RSTR;
  _vo uint32_t   APB2RSTR;
   uint32_t      Reserved2[2];

  _vo uint32_t   AHB1ENR;
  _vo uint32_t   AHB2ENR;
  uint32_t       Reserved3[2];
  _vo uint32_t   APB1ENR;
  _vo uint32_t   APB2ENR;

  uint32_t       Reserved4[2];
  _vo uint32_t   AHB1LPENR;
  _vo uint32_t   AHB2LPENR;
  uint32_t       Reserved5[2];
  _vo uint32_t   APB1LPENR;
  _vo uint32_t   APB2LPENR;
  uint32_t       Reserved6[2];
  _vo uint32_t   BDCR;
  _vo uint32_t   CSR;
  uint32_t       Reserved7[2];
  _vo uint32_t   SSCGR;
  _vo uint32_t   PLLI2SCFGR;
  _vo uint32_t   DCKCFGR;

}RCC_Reg_t;

/*
 *   RCC Pointer
 */
#define RCC              ((RCC_Reg_t*)RCC_PERIPH_BASE_ADDR)

/*
 *   GPIO Clock Enable
 */
#define GPIOA_CLK_EN() (RCC->AHB1ENR|=(1<<0))// Set Bit0 for GPIOA
#define GPIOB_CLK_EN() (RCC->AHB1ENR|=(1<<1))// Set Bit1 for GPIOB
#define GPIOC_CLK_EN() (RCC->AHB1ENR|=(1<<2))// Set Bit2 for GPIOC
#define GPIOD_CLK_EN() (RCC->AHB1ENR|=(1<<3))// Set Bit3 for GPIOD
#define GPIOE_CLK_EN() (RCC->AHB1ENR|=(1<<4))// Set Bit4 for GPIOE
#define GPIOH_CLK_EN() (RCC->AHB1ENR|=(1<<7))// Set Bit7 for GPIOH
/*
 *   GPIO Clock Disable
 */
#define GPIOA_CLK_DI() (RCC->AHB1ENR&=~(1<<0))// Clear Bit0 for GPIOA
#define GPIOB_CLK_DI() (RCC->AHB1ENR&=~(1<<1))// Clear Bit1 for GPIOB
#define GPIOC_CLK_DI() (RCC->AHB1ENR&=~(1<<2))// Clear Bit2 for GPIOC
#define GPIOD_CLK_DI() (RCC->AHB1ENR&=~(1<<3))// Clear Bit3 for GPIOD
#define GPIOE_CLK_DI() (RCC->AHB1ENR&=~(1<<4))// Clear Bit4 for GPIOE
#define GPIOH_CLK_DI() (RCC->AHB1ENR&=~(1<<7))// Clear Bit7 for GPIOH

/*
 *   SPI Clock Enable
 */
#define SPI1_CLK_EN()  (RCC->APB2ENR|=(1<<12))// Set Bit12 for SPI1
#define SPI4_CLK_EN()  (RCC->APB2ENR|=(1<<13))// Set Bit13 for SPI4
#define SP12_CLK_EN()  (RCC->APB1ENR|=(1<<14))// Set Bit14 for SPI2
#define SPI3_CLK_EN()  (RCC->APB1ENR|=(1<<15))// Set Bit15 for SPI3
/*
 *   SPI Clock Disable
 */
#define SP1_CLK_DI()  (RCC->APB2ENR&=~(1<<12))// Clear Bit12 for SPI1
#define SPI4_CLK_DI() (RCC->APB2ENR&=~(1<<13))// Clear Bit13 for SPI4
#define SP12_CLK_DI() (RCC->APB1ENR&=~(1<<14))// Clear Bit14 for SPI2
#define SPI3_CLK_DI() (RCC->APB1ENR&=~(1<<15))// Clear Bit15 for SPI3

/*
 *   I2C Clock Enable
 */
#define I2C1_CLK_EN() (RCC->APB1ENR|=(1<<21))// Set Bit21 for I2C1
#define I2C2_CLK_EN() (RCC->APB1ENR|=(1<<22))// Set Bit22 for I2C2
#define I2C3_CLK_EN() (RCC->APB1ENR|=(1<<23))// Set Bit23 for I2C3
/*
 *   I2C Clock Disable
 */
#define I2C1_CLK_DI() (RCC->APB1ENR&=~(1<<21))// Clear Bit21 for I2C1
#define I2C2_CLK_DI() (RCC->APB1ENR&=~(1<<22))// Clear Bit22 for I2C2
#define I2C3_CLK_DI() (RCC->APB1ENR&=~(1<<23))// Clear Bit23 for I2C3

/*
 *   USART Clock Enable
 */
#define USART1_CLK_EN() (RCC->APB2ENR|=(1<<4))//  Set Bit4 for USART1
#define USART6_CLK_EN() (RCC->APB2ENR|=(1<<5))//  Set Bit5 for USART6
#define USART2_CLK_EN() (RCC->APB1ENR|=(1<<17))// Set Bit17 for USART2
/*
 *   USART Clock Disable
 */
#define USART1_CLK_DI() (RCC->APB2ENR&=~(1<<4))//  Clear Bit4 for USART1
#define USART6_CLK_DI() (RCC->APB2ENR&=~(1<<5))//  Clear Bit5 for USART6
#define USART2_CLK_DI() (RCC->APB1ENR&=~(1<<17))// Clear Bit17 for USART2


/*
 *   ADC register definition
 */
typedef struct
{
	 _vo uint32_t  SR;
	 _vo uint32_t  CR1;
	 _vo uint32_t  CR2;
	 _vo uint32_t  SMPR1;
	 _vo uint32_t  SMPR2;
	 _vo uint32_t  JOFR1;
	 _vo uint32_t  JOFR2;
	 _vo uint32_t  JOFR3;
	 _vo uint32_t  JOFR4;
	 _vo uint32_t  HTR;
	 _vo uint32_t  LTR;
	 _vo uint32_t  SQR1;
	 _vo uint32_t  SQR2;
	 _vo uint32_t  SQR3;
	 _vo uint32_t  JSQR;
	 _vo uint32_t  JDR1;
	 _vo uint32_t  JDR2;
	 _vo uint32_t  JDR3;
	 _vo uint32_t  JDR4;
	 _vo uint32_t  DR;

}ADC_Reg_t;
/*
 *   ADC Pointer
 */

#define ADC1         ((ADC_Reg_t*)ADC1_PERIPH_BASE_ADDR)
/*
 *   ADC Clock Enable
 */
#define ADC1_CLK_EN() (RCC->APB2ENR|=(1<<8))//  Set Bit8 for ADC1

/*
 *   ADC Clock Disable
 */
#define ADC1_CLK_DI() (RCC->APB2ENR&=~(1<<8))//  Clear Bit8 for ADC1

/*
 *   SYSCFG Register definition
 */
typedef struct
{
	_vo uint32_t MEMRMP;
	_vo uint32_t PMC;
	_vo uint32_t EXTICR[4];
	 uint32_t    RESERVED1[2];
	_vo uint32_t CMPCR;

}SYSCFG_Reg_t;

#define SYSCFG  ((SYSCFG_Reg_t*)(SYSCFG_PERIPH_BASE_ADDR))
/*
 *   SYSCFG Clock Enable
 */
#define SYSCFG_CLK_EN()      (RCC->APB2ENR|=(1<<14))

/*
 *   EXTI Register definition
 */
typedef struct
{
	_vo uint32_t IMR;
	_vo uint32_t EMR;
	_vo uint32_t RTSR;
	_vo uint32_t FTSR;
	_vo uint32_t SWIER;
	_vo uint32_t PR;

}EXTI_Reg_t;

#define EXTI ((EXTI_Reg_t*)(EXTI_PERIPH_BASE_ADDR))

/*
 *   NVIC Register definition
 */
typedef struct
{
	volatile uint32_t ISER[8];
	volatile uint32_t RESERVE0[24];
	volatile uint32_t ICER[8];
	volatile uint32_t RESERVE1[24];
	volatile uint32_t ISPR[8];
	volatile uint32_t RESERVE2[24];
	volatile uint32_t ICPR[8];
	volatile uint32_t RESERVE3[24];
	volatile uint32_t IABR[8];
	volatile uint32_t RESERVE4[56];
	volatile uint8_t IP[240];


}NVIC_Reg_t;

#define NVIC ((NVIC_Reg_t*)NVIC_PERIPH_BASE_ADDR)


/*
 *    Generic Macros
 */
#define ENABLE  1
#define DISABLE 0
/*
 *    Peripheral reset Macros
 */
#define GPIOA_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<0);\
	        	                    RCC->AHB1RSTR &= ~(1<<0);\
	        	                    }while(0)
#define GPIOB_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<1);\
	        	                    RCC->AHB1RSTR &= ~(1<<1);\
	        	                    }while(0)
#define GPIOC_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<2);\
	        	                    RCC->AHB1RSTR &= ~(1<<2);\
	                                }while(0)
#define GPIOD_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<3);\
	        	                    RCC->AHB1RSTR &= ~(1<<3);\
	        	                    }while(0)
#define GPIOE_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<4);\
	        	                    RCC->AHB1RSTR &= ~(1<<4);\
	        	                    }while(0)
#define GPIOH_REG_RESET()    do{\
                                    RCC->AHB1RSTR |=  (1<<7);\
	        	                    RCC->AHB1RSTR &= ~(1<<7);\
	        	                    }while(0)



#endif /* INC_STM32F401RE_H_ */
