/*
 * GPIOdriver.c
 *
 *  Created on: Jul 24, 2026
 *      Author: hp
 */
#include "stdint.h"
#include "STM32F401xx.h"
#include "GPIOdriver.h"

void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx, uint8_t EnOrDi)
{
	if(EnOrDi == ENABLE)
	{
		if (pGPIOx == GPIOA)
		{
		   GPIOA_PCLK_EN();
		}
		else if (pGPIOx == GPIOB)
		{
		   GPIOB_PCLK_EN();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_EN();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_EN();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_EN();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_EN();
		}
	}
	else
	{
		if (pGPIOx == GPIOA)
		{
			GPIOA_PCLK_DI();
		}
		else if (pGPIOx == GPIOB)
		{
			GPIOB_PCLK_DI();
		}
		else if (pGPIOx == GPIOC)
		{
			GPIOC_PCLK_DI();
		}
		else if (pGPIOx == GPIOD)
		{
			GPIOD_PCLK_DI();
		}
		else if (pGPIOx == GPIOE)
		{
			GPIOE_PCLK_DI();
		}
		else if (pGPIOx == GPIOH)
		{
			GPIOH_PCLK_DI();
		}
	}
}

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp = 0;

	    // 1. Configure the mode of GPIO pin
	    if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG)
	    {
	    	// Non-interrupt mode
	        temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	        // Clear the 2 bits first
	        pGPIOHandle->pGPIOx->MODER &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));

	        // Set the new value
	        pGPIOHandle->pGPIOx->MODER |= temp;
	    }

	    else
	    {
	    	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT)
	    	{
	    		EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    		EXTI->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    	}
	    	else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT)
	    	{
	    		EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    	    EXTI->FTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    	}
	    	else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT)
	    	{
	    		EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    	    EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    	}

	    uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
	    uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;

	    uint8_t value = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
	    SYSCFG_PCLK_EN();
	    SYSCFG->EXTICR[temp1] |= value << (4 * temp2);
	    EXTI->IMR |= ( 1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);

	    }

	    // 2. Configure the speed
	    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	    pGPIOHandle->pGPIOx->OSPEEDR |= temp;
	    temp = 0;

	    // 3. Configure the pull-up/pull-down settings
	    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdContol << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3 << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	    pGPIOHandle->pGPIOx->PUPDR |= temp;
	    temp = 0;

	    // 4. Configure the output type (only 1 bit per pin)
	    temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    pGPIOHandle->pGPIOx->OTYPER &= ~(0x1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	    pGPIOHandle->pGPIOx->OTYPER |= temp;
	    temp = 0;
}

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx)
{
	if (pGPIOx == GPIOA)
	{
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB)
	{
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC)
	{
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD)
	{
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE)
	{
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOH)
	{
		GPIOH_REG_RESET();
	}
}

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	    // Read IDR, shift right by pin number, mask with 1
	    value = (uint8_t)((pGPIOx->IDR >> PinNumber) & 0x00000001);

	    return value;
}

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value)
{
	if (Value == GPIO_PIN_SET)
	    {
	        // Set the bit
	        pGPIOx->ODR |= (1 << PinNumber);
	    }
	    else
	    {
	        // Clear the bit
	        pGPIOx->ODR &= ~(1 << PinNumber);
	    }
}

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx)
{
	uint16_t value;
    value = (uint16_t)pGPIOx->IDR;
	return value;
}

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx, uint16_t Value)
{
	pGPIOx->ODR = Value;
}

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1 << PinNumber);  // XOR to toggle
}

void GPIO_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi)
{
    if(EnorDi == ENABLE)
    {
        // Enable interrupt in NVIC
        if(IRQNumber <= 31)
        {
            // Use ISER0 for IRQ 0-31
            *NVIC_ISER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            // Use ISER1 for IRQ 32-63
            *NVIC_ISER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            // Use ISER2 for IRQ 64-95
            *NVIC_ISER2 |= (1 << (IRQNumber % 64));
        }
    }
    else
    {
        // Disable interrupt in NVIC
        if(IRQNumber <= 31)
        {
            *NVIC_ICER0 |= (1 << IRQNumber);
        }
        else if(IRQNumber > 31 && IRQNumber < 64)
        {
            *NVIC_ICER1 |= (1 << (IRQNumber % 32));
        }
        else if(IRQNumber >= 64 && IRQNumber < 96)
        {
            *NVIC_ICER2 |= (1 << (IRQNumber % 64));
        }
    }
}

void GPIO_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority)
{
    // Calculate which IPR register (each handles 4 IRQs)
    uint8_t iprx = IRQNumber / 4;

    // Calculate which byte within IPR register (0-3)
    uint8_t iprx_section = IRQNumber % 4;

    // Calculate bit shift amount
    // Each IRQ uses 8 bits, but only upper 4 bits are implemented
    uint8_t shift_amount = (8 * iprx_section) + (8 - NO_PR_BITS_IMPLEMENTED);

    // Set priority value (clear old value first for safety)
    *(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);
}

void GPIO_IRQHandling(uint8_t PinNumber)
{
    // Check if interrupt is pending for this pin
    if(EXTI->PR & (1 << PinNumber))
    {
        // Clear the pending bit by writing 1 to it (RC_W1 type)
        EXTI->PR |= (1 << PinNumber);
    }
}

