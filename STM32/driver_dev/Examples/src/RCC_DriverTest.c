/*
 * RCC_DriverTest.c
 *
 *  Created on: Jul 8, 2026
 *      Author: hp
 */


#include "RCC_DriverTest.h"

int RCCTest(void)
{

	// Initialize RCC oscillators
	RCC_OscConfig_t Osc;
	Osc.HSE_State = RCC_HSE_OFF;
	Osc.HSI_State = RCC_HSI_ON;
	//Osc.HSI_State = RCC_HSI_OFF; not possible: after reset, MCU starts with SWS = HSI, so OscConfig() cannot disable it.
	Osc.PLL.PLL_State = RCC_PLL_ON;
	Osc.PLL.PLL_Source = RCC_PLLSOURCE_HSI;
	Osc.PLL.PLLM = 16;
	Osc.PLL.PLLN = 300;
	Osc.PLL.PLLP = RCC_PLLP_DIV4;

	RCC_Status_t Oscstatus = RCC_OscConfig(&Osc);

	// Initialize CPU, AHB abd APB bus clocks
	RCC_ClockConfig_t   clk;
	clk.SysClock      = RCC_SYSCLKSOURCE_PLL;
	clk.AHBPrescaler  = RCC_AHB_DIV1;
	clk.APB1Prescaler = RCC_APB_DIV2;
	clk.APB2Prescaler = RCC_APB_DIV1;

	RCC_Status_t clkstatus =RCC_SysClockConfig(&clk);

	// Read clock frequencies for validation
	uint32_t sysclk  = RCC_GetSysClockFreq();
	uint32_t ahbclk  = RCC_GetHCLKFreq();
	uint32_t apb1clk = RCC_GetPCLK1Freq();
	uint32_t apb2clk = RCC_GetPCLK2Freq();
	uint32_t pllclk  = RCC_GetPLLFreq();

	// Print clock related register configurations and clock frequencies

	while(1)
	{
		printf("\n\n\n      RCC Driver Testing");
	    printf("\n****************************************");
		printf("\n\nCPU, AHB and APB bus clocks:");
		printf("\n    SYSCLK  = %lu ",   sysclk);
		printf("\n    AHBCLK  = %lu ",   ahbclk);
		printf("\n    APB1CLK = %lu ",   apb1clk);
		printf("\n    APB2CLK = %lu ",   apb2clk);
		printf("\n    PLLCLK  = %lu ",   pllclk);

		printf("\n\nAPI status:");
		printf("\n    OscillatorConfiguration status : %d", Oscstatus);// 1 - RCC_OK, 0 - RCC_ERROR
		printf("\n    ClockConfiguration status      : %d", clkstatus);

		printf("\n\nRegister status bits:");
		printf("\n    SWS        : %lu",        (RCC->CFGR >> 2)&0x3);// 00-HSI, 01-HSE, 10-PLL
		printf("\n    HSE state  : %lu", ((RCC->CR>>16)&1));   // 0- OFF, 1-ON
		printf("\n    HSE bypass : %lu", ((RCC->CR>>18)&1));   // 0- OFF, 1-ON
		printf("\n    HSI state  : %lu", ((RCC->CR>>0)&1));    // 0- OFF, 1-ON
		printf("\n    PLL state  : %lu", ((RCC->CR>>24)&1));   // 0- OFF, 1-ON
		printf("\n    PLL ready  : %lu", ((RCC->CR>>25)&1));   // 0- Not Locked, 1-READY
		printf("\n    PLL source : %lu", ((RCC->PLLCFGR>>22)&1));// 0-HSI, 1-HSE

		printf("\n\nRegister values:");
		printf("\n    CR         : 0x%08lX", RCC->CR);
		printf("\n    CFGR       : 0x%08lX", RCC->CFGR);
		printf("\n    PLLCFGR    : 0x%08lX", RCC->PLLCFGR);

		printf("\n\nFlash Interface:");
		printf("\n    FLASH Latency: %lu", (FLASH->ACR & 0xF));// number of wait states

		//force delay
		for(int i=0; i<200000;i++);
	}
}
void RCCConfigurePLL_84MHz()
{
	// for 84MHz PLL output,
	// PLLM = 16, PLLN = 336, PLLP = 4
	// PLL output = (HSI_Freqency / 16 ) * (336 / 4)

	// Configure Oscillator
	RCC_OscConfig_t Osc;
	Osc.HSE_State = RCC_HSE_OFF;
	Osc.HSI_State = RCC_HSI_ON;
	Osc.PLL.PLLM = 16;
	Osc.PLL.PLLN = 336;
	Osc.PLL.PLLP = RCC_PLLP_DIV4;
	Osc.PLL.PLL_State = RCC_PLL_ON;
	Osc.PLL.PLL_Source= RCC_PLLSOURCE_HSI;
	RCC_OscConfig(&Osc);

	// configure Clock
	RCC_ClockConfig_t clk;
	clk.SysClock = RCC_SYSCLKSOURCE_PLL;
	clk.AHBPrescaler = RCC_AHB_DIV1;
	clk.APB1Prescaler = RCC_APB_DIV2;
	clk.APB2Prescaler = RCC_APB_DIV1;
	RCC_SysClockConfig(&clk);

}


