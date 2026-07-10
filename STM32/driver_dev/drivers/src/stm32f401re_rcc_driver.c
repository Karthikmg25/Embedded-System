/*
 * stm32f401re_rcc_driver.c
 *
 *  Created on: Jul 2, 2026
 *      Author: hp
 */

#include "stm32f401re_rcc_driver.h"

/*
 * Prescaler lookup tables.
 *
 * These arrays map the encoded prescaler bit values from RCC_CFGR/PLLCFGR
 * registers to their corresponding division factors.
 */
static const uint16_t AHBPrescaler[]={ 2, 4, 8, 16, 64, 128, 256, 512};
static const uint16_t APBPrescaler[]={ 2, 4, 8, 16};
/*
 * PLLP division factor lookup table.
 *
 * Maps the PLLP bit field (PLLP[1:0] in RCC_PLLCFGR) to the actual
 * PLL output division factor:
 *   00 -> /2
 *   01 -> /4
 *   10 -> /6
 *   11 -> /8
 */
static const uint8_t PLLPPrescaller[]={2, 4, 6, 8};

/**
 * @brief:  Returns the current system clock (SYSCLK) frequency.
 *
 * Reads the System Clock Switch Status (SWS) bits in the RCC CFGR register
 * to determine the active system clock source and returns its frequency.
 * If the PLL is selected as the system clock, the PLL output frequency is
 * calculated using the current PLL configuration.
 *
 * Clock source selection:
 * - HSI : Returns HSI_Freq.
 * - HSE : Returns HSE_Freq.
 * - PLL : Returns the frequency computed by RCC_GetPLLFreq().
 *
 * @param : none
 *
 * @return: Current SYSCLK frequency in Hertz (Hz). Returns 0 if the SWS
 *          field contains a reserved value.
 *
 */
uint32_t RCC_GetSysClockFreq()
{
	// To read current system clock, read SWS(bit 3:2) of CFGR register
	//  00 - HSI frequency
	//  01 - HSE frequency
	//  10 - PLL frequency

	uint32_t clksrc, sysclk;

	clksrc = (RCC->CFGR >> 2) & 0x3;

	switch(clksrc)
	{
	    case RCC_SYSCLKSOURCE_HSI : sysclk= HSI_Freq;
	              break;
	    case  RCC_SYSCLKSOURCE_HSE : sysclk= HSE_Freq;
	              break;
	    case  RCC_SYSCLKSOURCE_PLL : sysclk= RCC_GetPLLFreq();
	               break;
	    case  3 : sysclk= 0;
	               break;
	}

	return sysclk;
}
/**
 * @brief:  Returns the current AHB bus clock (HCLK) frequency.
 *
 * Determines the current HCLK frequency by reading the AHB prescaler
 * (HPRE) bits in the RCC CFGR register and applying the corresponding
 * division factor to the current system clock (SYSCLK).
 *
 * If the AHB prescaler is configured as DIV1, HCLK is equal to SYSCLK.
 * Otherwise, HCLK is calculated by dividing SYSCLK by the configured
 * AHB prescaler.
 *
 * @param : none
 *
 * @return: Current HCLK frequency in Hertz (Hz).
 *
 */
uint32_t RCC_GetHCLKFreq()
{

	uint32_t sysclk,hpre, hclk;

	sysclk = RCC_GetSysClockFreq();

	// read HPRE bits(bit 7:4) in CFGR register to get AHB prescaler
    hpre = (RCC->CFGR >> 4) & 0xF;

	if(hpre <8)
	{
		// No system clock division
	    hclk = sysclk;
	}
	else
	{
		hclk = sysclk / AHBPrescaler[hpre-8];
	}
	return hclk;
}
/**
 * @brief:  Returns the current APB1 peripheral clock (PCLK1) frequency.
 *
 * Determines the current PCLK1 frequency by reading the APB1 prescaler
 * (PPRE1) bits in the RCC CFGR register and applying the corresponding
 * division factor to the current AHB clock (HCLK).
 *
 * If the APB1 prescaler is configured as DIV1, PCLK1 is equal to HCLK.
 * Otherwise, PCLK1 is calculated by dividing HCLK by the configured
 * APB1 prescaler.
 *
 * @param  : none
 * @return : Current PCLK1 frequency in Hertz (Hz).
 *
 */
uint32_t RCC_GetPCLK1Freq()
{
	uint32_t ppre1, hclk, pclk1;

	hclk  = RCC_GetHCLKFreq();

	// read PPRE1 bits in CFGR register to get APB1 prescaler
	ppre1 = (RCC->CFGR >> 10) & 0x7;

	if(ppre1 < 4)
	{
		// No APB1 clock division
	    pclk1 = hclk;
	}
	else
	{
		pclk1 = hclk / APBPrescaler[ppre1-4];
	}
	return pclk1;

}
/**
 * @brief:  Returns the current APB2 peripheral clock (PCLK2) frequency.
 *
 * Determines the current PCLK2 frequency by reading the APB2 prescaler
 * (PPRE2) bits in the RCC CFGR register and applying the corresponding
 * division factor to the current AHB clock (HCLK).
 *
 * If the APB2 prescaler is configured as DIV1, PCLK2 is equal to HCLK.
 * Otherwise, PCLK2 is calculated by dividing HCLK by the configured
 * APB2 prescaler.
 *
 * @param  : none
 *
 * @return : Current PCLK2 frequency in Hertz (Hz).
 */
uint32_t RCC_GetPCLK2Freq()
{
	uint32_t ppre2, hclk, pclk2;

	hclk  = RCC_GetHCLKFreq();

	// read PPRE2 bits in CFGR register to get APB2 prescaler
	ppre2 = (RCC->CFGR >> 13) & 0x7;

	if(ppre2 < 4)
	{
		// No APB2 clock division
	    pclk2 = hclk;
	}
	else
	{
		pclk2 = hclk / APBPrescaler[ppre2-4];
	}
	return pclk2;

}
/**
 * @brief:  Returns the current PLL output clock frequency.
 *
 * Calculates the PLL output frequency by reading the PLL configuration
 * registers (PLLCFGR). The function determines the PLL clock source,
 * computes the VCO input and VCO output frequencies, and finally
 * calculates the PLL output frequency using the configured PLLP divider.
 *
 * The PLL output frequency is calculated as:
 * VCO Input  = PLL Source / PLLM
 * VCO Output = VCO Input × PLLN
 * PLL Output = VCO Output / PLLP
 *
 * @param : none
 *
 * @note  : This function returns the PLL output frequency regardless of
 *       whether the PLL is selected as the system clock.
 *
 * @return: PLL output frequency in Hertz (Hz). Returns 0 if the PLL is
 *         disabled or an invalid PLL configuration is detected.
 */
uint32_t RCC_GetPLLFreq()
{
	uint32_t pllsrc, VCOinput, VCOoutput, PLLoutput ;
	uint16_t PLLM, PLLN, PLLP;

	// Check if PLL enabled
	if((RCC->CR & (1<<24)) == 0)
	{
		//PLL disabled
		return 0;
	}

	// 1. check PLL clock source
	if(((RCC->PLLCFGR >> 22)&1) == 0)
	{
		pllsrc = HSI_Freq;
	}
	else
	{
		pllsrc = HSE_Freq;
	}

	// 2. Find VCO input frequency:
	//   - VCO i/p = pllsrc / PLLM
	//   - PLLM = bit5:0 in PLLCFGR

	PLLM     = RCC->PLLCFGR & 0x3F;
	if(PLLM < 2)
	{
		//wrong configuration
		return 0;
	}
	VCOinput = pllsrc / PLLM;

	// 3. Find VCO output frequency:
	//   - VCO o/p = VCO i/p * PLLN
	//   - PLLN = bit14:6 in PLLCFGR

	PLLN     = (RCC->PLLCFGR >> 6) & 0x1FF;
	if(PLLN < 2 || PLLN > 432)
	{
		//wrong configuration
		return 0;
	}
	VCOoutput = VCOinput * PLLN;

	// 4. Find PLL output frequency
	//   - PLL output = VCO output / PLLP
	//   - PLLP = bit17:16 in PLLCFGR

	PLLP  = (RCC-> PLLCFGR >> 16)& 0x3;

	PLLoutput = VCOoutput / PLLPPrescaller[PLLP];

	return PLLoutput;
}

//****************************************** Helper functions ****************************************************************************

// Clock prescaler Configuration helpers
static void RCC_SetAHBPrescaler(uint8_t prescaler)
{
	// Configure the AHB prescaler (HPRE bits 7:4 in CFGR).
	RCC->CFGR &=~(0xF<< 4);
	RCC->CFGR |= (prescaler<< 4);

}
static void RCC_SetAPB1Prescaler(uint8_t prescaler)
{
	// Configure the APB1 prescaler (PPRE1 bits 12:10 in CFGR).
		RCC->CFGR &=~(0x7<< 10);
		RCC->CFGR |= (prescaler<< 10);
}
static void RCC_SetAPB2Prescaler(uint8_t prescaler)
{
	// Configure the APB2 prescaler (PPRE2 bits 15:13 in CFGR).
			RCC->CFGR &=~(0x7<< 13);
			RCC->CFGR |= (prescaler<< 13);
}

// PLL Configuration helpers
static void RCC_SetPLLM(uint16_t PLLM)
{
	// set PLLN (bit5:0 in PLLCFGR register)
	RCC->PLLCFGR &=~(0x3F<< 0);
	RCC->PLLCFGR |= (PLLM<< 0);

}
static void RCC_SetPLLN(uint16_t PLLN)
{
	// set PLLM (bit14:6 in PLLCFGR register)
	RCC->PLLCFGR &=~(0x1FF<< 6);
	RCC->PLLCFGR |= (PLLN<< 6);
}
static void RCC_SetPLLP(uint16_t PLLP)
{
	// set PLLP (bit17:16 in PLLCFGR register)
	RCC->PLLCFGR &=~(0x3 << 16);
	RCC->PLLCFGR |= (PLLP<< 16);
}
static void RCC_SetPLLSource(uint8_t PLLSource)
{

	// set PLL source
	// 0 - HSI as PLL source
	// 1 - HSE as PLL source
	if(PLLSource == RCC_PLLSOURCE_HSI)
	{
		RCC->PLLCFGR &= ~RCC_PLLCFGR_PLLSRC;
	}
	else
	if(PLLSource == RCC_PLLSOURCE_HSE)
	{
		RCC->PLLCFGR |= RCC_PLLCFGR_PLLSRC;
	}
}
// Osicllator enable and disable helpers
static void RCC_EnableHSI()
{
	// 1. enable HSI oscillator
	RCC->CR |= (RCC_CR_HSION);

	// 2. wait till HSI is ready/stable
	while((RCC->CR & RCC_CR_HSIRDY) == 0);

}
static void RCC_EnableHSE(uint8_t hsestate)
{

	// Enable the HSE oscillator in crystal or bypass mode

	// Select HSE operating mode.
	    // - RCC_HSE_ON     : External crystal
	    // - RCC_HSE_BYPASS : External MCO clock source.
	if(hsestate == RCC_HSE_BYPASS)
	{
		// Configure bypass mode
		RCC->CR |= (RCC_CR_HSEBYP);
	}
	else
	{
		// Disable bypass mode
		RCC->CR &= ~(RCC_CR_HSEBYP);
	}


	// 1. enable HSE Oscillator
	RCC->CR |= (RCC_CR_HSEON);

	// 2. wait till HSE is ready/stable
	while((RCC->CR & RCC_CR_HSERDY) == 0);

}
static void RCC_EnablePLL()
{
	// 1. enable the PLL
	RCC->CR |= (RCC_CR_PLLON);

	// 2. wait till PLL is ready/stable
	while((RCC->CR & RCC_CR_PLLRDY) == 0);

}
// Oscillator disable functions
static void RCC_DisableHSI()
{

	// disable HSI
	RCC->CR &= ~(RCC_CR_HSION);

}
static void RCC_DisableHSE()
{
	// disable HSE
	RCC->CR &= ~((RCC_CR_HSEON) | (RCC_CR_HSEBYP));

}
static void RCC_DisablePLL()
{
	// disable PLL
	RCC->CR &= ~(RCC_CR_PLLON);

	// wait till PLL is disabled before configurations
	while((RCC->CR & RCC_CR_PLLRDY) != 0);

}

/*
 * Configure the Flash memory wait states based on the HCLK frequency.
 *
 * Flash access time is fixed, while the CPU clock (HCLK) can vary.
 * As HCLK increases, additional wait states are required to ensure
 * reliable instruction and data fetches from Flash memory.
 *
 * Wait states for STM32F401:
 *   HCLK ≤ 30 MHz  : 0 wait states
 *   HCLK ≤ 60 MHz  : 1 wait state
 *   HCLK ≤ 84 MHz  : 2 wait states
 */
static void RCC_SetFlashWaitstates(uint32_t HCLKFreq)
{
	if(HCLKFreq <= 30000000)
	{
		// no wait states required
		FLASH->ACR &=~(0xF<< 0);

	}
	else if(HCLKFreq >30000000 && HCLKFreq <= 60000000)
	{
		// 1 wait state
		FLASH->ACR &=~(0xF<< 0);
		FLASH->ACR |= (0x1<< 0);

	}
	else if(HCLKFreq >60000000 && HCLKFreq <= 84000000)
	{
		// 2 wait states
		FLASH->ACR &=~(0xF<< 0);
		FLASH->ACR |= (0x2<< 0);

	}

}
/*
 * Calculate the future HCLK frequency that will result from the requested
 * system clock source and AHB prescaler configuration.
 */
static uint32_t RCC_CalculateFutureHCLK(RCC_ClockConfig_t *clk)
{


	uint32_t futuresysclk, futurehclk;
	if(clk->SysClock == RCC_SYSCLKSOURCE_HSI)
	{
		futuresysclk = HSI_Freq;
	}
	else  if(clk->SysClock == RCC_SYSCLKSOURCE_HSE)
	{
		futuresysclk = HSE_Freq;
	}
	else
	{
		// This function is called after configuring PLLCFGR register
		// so GetPLLFreq() provides PLL frequency after configuration
		futuresysclk = RCC_GetPLLFreq();
	}

	// future AHB prescaler
	uint16_t hpre = clk->AHBPrescaler;

	if(hpre < 8)
	{
		futurehclk = futuresysclk;
	}
	else
	{
		futurehclk = futuresysclk / AHBPrescaler[hpre-8];
	}


	return futurehclk;
}

//*****************************************************************************************************************************************

/**
 * @brief:  Configures the system clock source and bus prescalers.
 *
 * Configures the AHB, APB1, and APB2 prescalers and switches the system
 * clock (SYSCLK) to the selected clock source (HSI, HSE, or PLL).
 *
 * Before switching the system clock, the function verifies that the
 * requested clock source is enabled and stable. It also validates the
 * resulting bus clock frequencies and adjusts the Flash wait states,
 * when required, to ensure reliable operation.
 *
 * Flash latency is increased before switching to a higher HCLK frequency
 * and reduced after switching to a lower HCLK frequency.
 *
 * @param[in]:  pClkConfig - Pointer to the system clock configuration structure.
 *
 * @return   :  RCC_OK on success.
 *              RCC_ERROR if the requested clock source is not ready.
 *              RCC_ERROR_INVALID_APB1CLK if the requested APB1 clock exceeds
 *                                    the maximum supported frequency.
 */
RCC_Status_t  RCC_SysClockConfig(RCC_ClockConfig_t *pClkConfig)
{

	//******************************************************
	// Calculate future clock frequencies
	//******************************************************

	uint32_t currenthclk = RCC_GetHCLKFreq();// CFGR not yet configured, so GetHCLKFreq() provides current HCLK
	uint32_t futurehclk = RCC_CalculateFutureHCLK(pClkConfig);
	uint32_t futureAPB1;
		if(pClkConfig->APB1Prescaler < 4)
		{
			futureAPB1 = futurehclk;
		}
		else
		{
			futureAPB1 = futurehclk / APBPrescaler[pClkConfig->APB1Prescaler - 4];
		}


	//******************************************************
	// Validate requested configuration
	//******************************************************

	    // check if future APB1 is larger than 42MHz: error
		if(futureAPB1 > 42000000)
		{
			return RCC_ERROR_INVALID_APB1CLK;
		}


	//******************************************************
	// Configure Flash latency before increasing the cpu clock
	//******************************************************

	//  If future AHB frequency is larger(eg: 16MHz -> 84MHz), configure Flash latency before system clock switch
	if(futurehclk > currenthclk)
	{
		RCC_SetFlashWaitstates(futurehclk);
	}


	//******************************************************
	// Configure AHB/APB prescalers
	//******************************************************

		 RCC_SetAHBPrescaler(pClkConfig->AHBPrescaler);
		 RCC_SetAPB1Prescaler(pClkConfig->APB1Prescaler);
		 RCC_SetAPB2Prescaler(pClkConfig->APB2Prescaler);


	//******************************************************
	// Switch system clock source
	//******************************************************

	  if(pClkConfig->SysClock ==  RCC_SYSCLKSOURCE_HSI)
	  {


		 // 1. check if HSI is locked/ stable before selection
         // clock source must be enabled and stable before swicthing
		  if((RCC->CR & RCC_CR_HSIRDY) == 0)
		 {
		 	return RCC_ERROR;
	     }

		  // 2. switch to HSI as clock source: set SW bits in CFGR as 00
		  RCC->CFGR &= ~(0x3 << 0);

		  // 3. wait for SWS after switching
          // wait till HSI is used as system clock
		 while (((RCC->CFGR >> 2) & 0x3) != 0);


	  }
	  else if(pClkConfig->SysClock ==  RCC_SYSCLKSOURCE_HSE)
	  {


		  //1. check if HSE is locked/stable before selection
		  // clock source must be enabled and stable before swicthing
		  if((RCC->CR & RCC_CR_HSERDY) == 0)
		  {
		  	return RCC_ERROR;
		  }
		  //2. switch to HSE as clock source: set SW bits in CFGR as 01
		 RCC->CFGR &= ~(0x3 << 0);
		 RCC->CFGR |=  (0x1 << 0);

		 //3. wait for SWS after switching
         // wait till HSE is used as system clock
	     while (((RCC->CFGR >> 2) & 0x3) != 1);


	  }
	  else if(pClkConfig->SysClock ==  RCC_SYSCLKSOURCE_PLL)
	  {


			  //1. check if PLL is locked before selection
		      // clock source must be enabled and stable before swicthing
			  if((RCC->CR & RCC_CR_PLLRDY) == 0)
			  {
			  	return RCC_ERROR;
			  }
			  //2. switch to PLL as clock source:set SW bits in CFGR as 10
			 RCC->CFGR &= ~(0x3 << 0);
			 RCC->CFGR |=  (0x2 << 0);

			 //3. wait for SWS after switching
             // wait till PLL is used as system clock
			 while (((RCC->CFGR >> 2) & 0x3) != 2);

	  }


	  //******************************************************
	  // Restore Flash latency (if reducing frequency)
	  //******************************************************

	  // configure Flash latency after system clock switch if future frequency is smaller(eg: 84MHz -> 16MHz)
	  	if(futurehclk < currenthclk)
	  	{
	  		RCC_SetFlashWaitstates(futurehclk);
	  	}


	  return RCC_OK;
}

/**
 * @brief : Configures the system oscillators and PLL.
 *
 * Enables or disables the HSI, HSE, and PLL oscillators according to the
 * requested configuration. When the PLL is enabled, the function validates
 * the requested PLL parameters, programs the PLL configuration registers,
 * and enables the PLL.
 *
 * The function ensures that:
 * - The selected PLL source oscillator is enabled.
 * - Oscillators are stable before being used.
 * - PLL input, VCO, and output frequencies satisfy the device limits.
 * - An oscillator currently used as the system clock or PLL source is not disabled.
 * - The PLL is not reconfigured while it is used as the system clock.
 *
 * @param[in]  : pOscConfig - Pointer to the oscillator configuration structure.
 *
 * @return     : RCC_OK on success.
 * @return       RCC_ERROR_xxx if the requested configuration is invalid or
 *               violates hardware constraints.
 */

RCC_Status_t RCC_OscConfig(RCC_OscConfig_t *pOscConfig)
{

	//*************************************************************************************************************************************
	// verify PLL source configuration
	//***********************************************************************************************************************

	if(pOscConfig->PLL.PLL_Source == RCC_PLLSOURCE_HSI && (pOscConfig->HSI_State == RCC_HSI_OFF))
	{
		// HSI is selected as PLL source but set as OFF state
		return RCC_ERROR;
	}
	if(pOscConfig->PLL.PLL_Source == RCC_PLLSOURCE_HSE && (pOscConfig->HSE_State == RCC_HSE_OFF))
	{
		// HSE is selected as PLL source but set as OFF state
		return RCC_ERROR;
	}

	//*************************************************************************************************************************************
	// 1. Enable requested oscillators
	//****************************************************************************************************************************************

	// HSI Enable
	if(pOscConfig->HSI_State == RCC_HSI_ON)
	{
		RCC_EnableHSI();
	}
	// HSE Enable
	if(pOscConfig->HSE_State == RCC_HSE_ON || pOscConfig->HSE_State == RCC_HSE_BYPASS)
	{
		RCC_EnableHSE(pOscConfig->HSE_State);
	}



	//*************************************************************************************************************************************
	// 2. Configure and Enable PLL if requested
	//*************************************************************************************************************************************

	if(pOscConfig->PLL.PLL_State == RCC_PLL_ON)
	{
		// steps:
		// 1. verification
		// 2. disable PLL if already enabled
		// 3. configure PLLSRC, PLLM, PLLN, PLLP
		// 4. enable PLL, wait till it's enabled.

		//            check PLL source, PLLM, PLLN, PLLP, VCO, PLL output
		//******************************************************************

		uint32_t PLLsourceFreq;
		if(pOscConfig->PLL.PLL_Source == RCC_PLLSOURCE_HSI)
		{
			PLLsourceFreq = HSI_Freq;
			// HSI is not enabled but selected as PLL source
			if((RCC->CR & RCC_CR_HSIRDY) == 0)
			return RCC_ERROR;
		}
		if(pOscConfig->PLL.PLL_Source == RCC_PLLSOURCE_HSE)
		{
			PLLsourceFreq = HSE_Freq;
			// HSE is not enabled but selected as PLL source
			if((RCC->CR & RCC_CR_HSERDY) == 0)
			return RCC_ERROR;
		}
		// check PLLM, PLLN, PLLP
		if(pOscConfig->PLL.PLLM < 2 || pOscConfig->PLL.PLLM > 63)
		{
			return RCC_ERROR;
		}
		if(pOscConfig->PLL.PLLN <2 || pOscConfig->PLL.PLLN > 432)
		{
			return RCC_ERROR;
		}
		if(pOscConfig->PLL.PLLP > RCC_PLLP_DIV8)
		{
	    	return RCC_ERROR;
		}
		//check if VCO i/p, VCO o/p and PLL frequencies are valid
		uint32_t VCOip, VCOop, PLLfrequency;
		VCOip = PLLsourceFreq / pOscConfig->PLL.PLLM;

		if( VCOip < 1000000 || VCOip > 2000000  )// 1-2 MHz
		{
			return RCC_ERROR;
		}
		VCOop = VCOip * pOscConfig->PLL.PLLN;

		if( VCOop < 192000000 || VCOop > 432000000  )// 192-432 MHz
		{
			return RCC_ERROR;
		}
		PLLfrequency = VCOop / PLLPPrescaller[pOscConfig->PLL.PLLP];
		if(  PLLfrequency > 84000000  )// <=84MHz
		{
			return RCC_ERROR_INVALID_PLLCLK;
		}

		//           disable PLL if already enabled
		//**********************************************************

		// check if PLL is used as current system clock: cannot be disabled
		if(RCC_GET_SYSCLK_SRC() == RCC_SYSCLKSOURCE_PLL)
		{
			return RCC_ERROR;
		}
		if(RCC->CR & RCC_CR_PLLON)
		{
			RCC_DisablePLL();

		}
		//            configure PLLSRC, PLLM, PLLN, PLLP
		//**********************************************************


		RCC_SetPLLSource(pOscConfig->PLL.PLL_Source);

		RCC_SetPLLM(pOscConfig->PLL.PLLM);

		RCC_SetPLLN(pOscConfig->PLL.PLLN);

		RCC_SetPLLP(pOscConfig->PLL.PLLP);


		//         enable PLL, wait till it's enabled.
		//**********************************************************

		RCC_EnablePLL();

	}

	//*************************************************************************************************************************************
	// 3. Disable unwanted oscillators
	//*************************************************************************************************************************************

	// HSI disable
	if(pOscConfig->HSI_State == RCC_HSI_OFF)
	{
		// before disabling HSI, check if HSI is used as system clock or as PLL source: cannot be disabled
			if(RCC_GET_SYSCLK_SRC() == RCC_SYSCLKSOURCE_HSI)
			{
				return RCC_ERROR;
			}
			if((RCC->CR & RCC_CR_PLLON) && (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC )== 0)
			{
				return RCC_ERROR;
			}

			RCC_DisableHSI();
	}

	// HSE disable
	if(pOscConfig->HSE_State == RCC_HSE_OFF)
	{
		// before disabling HSE, check if HSE is used as system clock or as PLL source: cannot be disabled
		if(RCC_GET_SYSCLK_SRC() == RCC_SYSCLKSOURCE_HSE)
		{
			return RCC_ERROR;
		}
		if((RCC->CR & RCC_CR_PLLON) && (RCC->PLLCFGR & RCC_PLLCFGR_PLLSRC ) != 0)
		{
			return RCC_ERROR;
		}

		RCC_DisableHSE();
	}
	// PLL Disable
	if(pOscConfig->PLL.PLL_State == RCC_PLL_OFF)
	{
		// before disabling PLL, check if PLL is currently used as systemclock
		if(RCC_GET_SYSCLK_SRC() == RCC_SYSCLKSOURCE_PLL)
		{
			return RCC_ERROR;
		}
		RCC_DisablePLL();
	}

	return RCC_OK;

}
























