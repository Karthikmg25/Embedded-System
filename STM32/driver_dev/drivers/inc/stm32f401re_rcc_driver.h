/*
 * stm32f401re_rcc_driver.h
 *
 *  Created on: Jul 2, 2026
 *      Author: hp
 */

#ifndef INC_STM32F401RE_RCC_DRIVER_H_
#define INC_STM32F401RE_RCC_DRIVER_H_

#include "STM32F401RE.h"

// define HSI and HSE frequencies

#define HSI_Freq     16000000
#define HSE_Freq     8000000


/*
 *  Structure definitions:
 ************************************************************************************************************************/
// for PLL configuration
typedef struct
{
	uint8_t  PLL_State;
	uint8_t  PLL_Source;
	uint16_t PLLM;
	uint16_t PLLN;
	uint8_t  PLLP;

}RCC_PLLConfig_t;

// for Oscillator configuration
typedef struct
{
	uint8_t HSI_State;
	uint8_t HSE_State;
	RCC_PLLConfig_t PLL;

}RCC_OscConfig_t;

// for clock configuration
typedef struct
{
	uint8_t SysClock;
	uint16_t AHBPrescaler;
	uint16_t APB1Prescaler;
	uint16_t APB2Prescaler;

}RCC_ClockConfig_t;

// Enum for RCC Status
typedef enum
{
	RCC_ERROR,
	RCC_OK,
	RCC_ERROR_INVALID_APB1CLK,
	RCC_ERROR_INVALID_PLLCLK

}RCC_Status_t;

/*
 *  Supported APIs
 ************************************************************************************************************************/

// API for Oscillator configurations
RCC_Status_t RCC_OscConfig(RCC_OscConfig_t *pOscConfig);

// API for clock configurations
RCC_Status_t  RCC_SysClockConfig(RCC_ClockConfig_t *pClkConfig);

// APIs for reading clock frequencies
uint32_t RCC_GetSysClockFreq();
uint32_t RCC_GetHCLKFreq();
uint32_t RCC_GetPCLK1Freq();
uint32_t RCC_GetPCLK2Freq();
uint32_t RCC_GetPLLFreq();


/*
 *  Macro definitions
 ************************************************************************************************************************/


// Register bits

//@CR
#define RCC_CR_HSIRDY  (1<< 1)
#define RCC_CR_HSERDY  (1<<17)
#define RCC_CR_PLLRDY  (1<<25)
#define RCC_CR_HSION   (1<< 0)
#define RCC_CR_HSEON   (1<< 16)
#define RCC_CR_PLLON   (1<< 24)
#define RCC_CR_HSEBYP   (1<< 18)
//@PLLCFGR
#define RCC_PLLCFGR_PLLM    0  // bit field - bit5:0
#define RCC_PLLCFGR_PLLN    6  // bit field - bit14:6
#define RCC_PLLCFGR_PLLQ    24 // bit field - bit5:0
#define RCC_PLLCFGR_PLLP    16 // bit field - bit17:16
#define RCC_PLLCFGR_PLLSRC  (1<< 22)
//@CFGR
#define RCC_CFGR_HPRE      4  // bitfield - bit7:4
#define RCC_CFGR_PPRE1     10 // bitfield - bit12:10
#define RCC_CFGR_PPRE2     13 // bitfield - bit15:13
#define RCC_CFGR_SW        0  // bitfield - bit1:0
#define RCC_CFGR_SWS       2  // bitfield - bit3:2


// Clock configuration

#define RCC_SYSCLKSOURCE_HSI     0
#define RCC_SYSCLKSOURCE_HSE     1
#define RCC_SYSCLKSOURCE_PLL     2
#define RCC_AHB_DIV1   0
#define RCC_AHB_DIV2   8
#define RCC_AHB_DIV4   9
#define RCC_AHB_DIV8   10
#define RCC_AHB_DIV16  11
#define RCC_AHB_DIV64  12
#define RCC_AHB_DIV128 13
#define RCC_AHB_DIV256 14
#define RCC_AHB_DIV512 15
#define RCC_APB_DIV1   0
#define RCC_APB_DIV2   4
#define RCC_APB_DIV4   5
#define RCC_APB_DIV8   6
#define RCC_APB_DIV16  7

// Oscillator configuration

#define RCC_HSI_OFF    0
#define RCC_HSI_ON     1
#define RCC_HSE_OFF    2
#define RCC_HSE_ON     3
                               // What is HSE BYPASS mode?
#define RCC_HSE_BYPASS 4       // - for STM32F401RE nucleo, there is no external crystal
                               // - it uses MCO output of STlink MCU (8MHz) as HSE
                               // - so, HSE must be configured in bypass mode
#define RCC_PLL_OFF    5
#define RCC_PLL_ON     6
#define RCC_PLLSOURCE_HSI  0
#define RCC_PLLSOURCE_HSE  1
#define RCC_PLLP_DIV2  0
#define RCC_PLLP_DIV4  1
#define RCC_PLLP_DIV6  2
#define RCC_PLLP_DIV8  3

// helper macros

#define RCC_GET_SYSCLK_SRC()   ((RCC->CFGR>>2) & 0x3)


#endif /* INC_STM32F401RE_RCC_DRIVER_H_ */
