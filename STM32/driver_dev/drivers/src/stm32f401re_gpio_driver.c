/*
 * stm32f401re_gpio_driver.c
 *
 *  Created on: Jan 29, 2026
 *      Author: hp
 */
#include <stdint.h>
#include "stm32f401re_gpio_driver.h"

/*************************************************************************
 * @fn                - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - pGPIOx: base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - EnorDi: ENABLE or DISABLE macros
 *
 * @return            - none
 *
 * @Note              - Refer macros for GPIO configurations
 */
void GPIO_PeriClockControl(GPIO_Reg_t *pGPIOx, uint8_t EnorDi)
{
	if(EnorDi==ENABLE)
	{
	        if(pGPIOx== GPIOA)
              {
        	    GPIOA_CLK_EN();
              }
	  else  if(pGPIOx== GPIOB)
	          {
	          	GPIOB_CLK_EN();
	          }

	  else  if(pGPIOx== GPIOC)
	          {
	          	GPIOC_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOD)
	          {
	          	GPIOD_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOE)
	          {
	          	GPIOE_CLK_EN();
	          }
	  else  if(pGPIOx== GPIOH)
	          {
	          	GPIOH_CLK_EN();
	          }
	}
	else
	{
		            if(pGPIOx== GPIOA)
		              {
		        	    GPIOA_CLK_DI();
		              }
			  else  if(pGPIOx== GPIOB)
			          {
			          	GPIOB_CLK_DI();
			          }

			  else  if(pGPIOx== GPIOC)
			          {
			          	GPIOC_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOD)
			          {
			          	GPIOD_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOE)
			          {
			          	GPIOE_CLK_DI();
			          }
			  else  if(pGPIOx== GPIOH)
			          {
			          	GPIOH_CLK_DI();
			          }

	}
}
/*************************************************************************
 * @fn                - GPIO_Read_InputPin
 *
 * @brief             - This function reads value in a GPIO pin
 *
 * @param[in]         - pGPIOx   : base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - PinNumber: ENABLE or DISABLE macros
 *
 * @return            - uint8_t
 *
 * @Note              - none
 */
uint8_t GPIO_Read_InputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber)
{
	uint8_t value;

	value= (uint8_t)((pGPIOx->IDR)>>PinNumber)&1;// read value from pin

	return value;
}

/*************************************************************************
 * @fn                - GPIO_Read_InputPort
 *
 * @brief             - This function reads value in a GPIO pin
 *
 * @param[in]         - pGPIOx   : base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 *
 * @return            - uint16_t
 *
 * @Note              - none
 */

uint16_t GPIO_Read_InputPort(GPIO_Reg_t *pGPIOx)
{
	uint16_t value = (uint16_t)pGPIOx->IDR;

	return value;
}

/*************************************************************************
 * @fn                - GPIO_Write_OutputPin
 *
 * @brief             - This function writes a value to a GPIO pin
 *
 * @param[in]         - pGPIOx   : base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - PinNumber
 * @param[in]         - value
 *
 * @return            - none
 *
 * @Note              - none
 */

void GPIO_Write_OutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber, uint8_t value)
{
	if(value==GPIO_PIN_HIGH)
	{
		pGPIOx->ODR |= (1<< PinNumber);
	}
	else
	{
		pGPIOx->ODR &= ~(1<< PinNumber);
	}
}
/*************************************************************************
 * @fn                - GPIO_Write_OutputPort
 *
 * @brief             - This function writes value to a GPIO port
 *
 * @param[in]         - pGPIOx   : base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - value
 *
 * @return            - none
 *
 * @Note              - none
 */
void GPIO_Write_OutputPort(GPIO_Reg_t *pGPIOx, uint16_t value)
{
	pGPIOx->ODR = value;
}

/*************************************************************************
 * @fn                - GPIO_Toggle_OutputPin
 *
 * @brief             - This function Toggles value in a GPIO pin
 *
 * @param[in]         - pGPIOx   : base address of the GPIO peripheral(GPIOA, GPIOB, .....GPIOH)
 * @param[in]         - PinNumber
 *
 * @return            - none
 *
 * @Note              - none
 */

void GPIO_Toggle_OutputPin(GPIO_Reg_t *pGPIOx, uint8_t PinNumber)
{
	pGPIOx->ODR ^= (1<< PinNumber );
}
/*************************************************************************
 * @fn                - GPIO_Init
 *
 * @brief             - This function initialize a GPIO pin
 *
 * @param[in]         - pGPIOHandle
 *
 * @return            - none
 *
 * @Note              - none
 */

void GPIO_Init(GPIO_Handle_t *pGPIOHandle)
{
	uint32_t temp=0;// to create a temperory mask

	//Enable peripheral clock

	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);

	// Configure mode : 32 bit register, 2 bits for each pin

    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode<=GPIO_MODE_ANALOG)
    {
    	temp= ((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode)<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));
    	pGPIOHandle->pGPIOx->MODER &= ~(0x3<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));// clear bit positions
    	pGPIOHandle->pGPIOx->MODER |= temp; // set mode
    	temp=0;// clear mask
    }
    else  // interrupt modes
    {
    	      if((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_FT))
    			{
    		        EXTI->FTSR |=  (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    		        EXTI->RTSR &= ~(1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    			}
    	      else
    	    	  if((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_RT))
  			      {
  		           EXTI->RTSR |=  (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  		           EXTI->FTSR &= ~(1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
  			      }
    	      else
    	    	  if((pGPIOHandle->GPIO_PinConfig.GPIO_PinMode==GPIO_MODE_IT_RFT))
       			  {
    	    	   EXTI->RTSR |=  (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	    	   EXTI->FTSR |=  (1<< pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
    	    	   }



    }
    // Configure speed : 32 bit register, 2 bits for each pin

    temp= ((pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed)<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));
    pGPIOHandle->pGPIOx->OSPEEDR &= ~(0x3<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));// clear bit positions
    pGPIOHandle->pGPIOx->OSPEEDR |= temp; // set speed
    temp=0;// clear mask

    // configure output type : 16 bit register, 1 bit for each pin

    temp= ((pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType)<<( pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));
    pGPIOHandle->pGPIOx->OTYPER&= ~(1<<( pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));// clear bit position
    pGPIOHandle->pGPIOx->OTYPER |= temp;// set output type
    temp=0;// clear mask

    // configure pull-up/pull-down : 32 bit register, 2 bits for each pin

    temp= ((pGPIOHandle->GPIO_PinConfig.GPIO_PinPUPD)<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));// create a mask
    pGPIOHandle->pGPIOx->PUPDR &= ~(0x3<<(2* pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber ));// clear bit positions
    pGPIOHandle->pGPIOx->PUPDR |= temp;// set PUPD
    temp=0;// clear mask

    // configure alternate function : two 32 bit registers , 4 bits for each pins
    //                                register[0]- pins 0-7, register[1]- pins 8-15

    if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN)// special case for alternate function setup
    {
    	uint8_t temp1, temp2;

    	temp1= pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 8;// 0 for pins0-7, 1 for pins8-15
    	temp2= pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 8;// common value to set bit positions
    	pGPIOHandle->pGPIOx->AFR[temp1] &=~(0x0F<<(4*temp2));  // clear bit positions
    	pGPIOHandle->pGPIOx->AFR[temp1] |=(pGPIOHandle->GPIO_PinConfig.GPIO_PINAlternate<< (4*temp2));// set ALT function
    }


}
/*************************************************************************
 * @fn                - GPIO_DeInit
 *
 * @brief             - This function De-initialize a GPIO
 *
 * @param[in]         - pGPIOx
 *
 * @return            - none
 *
 * @Note              - none
 */
void GPIO_DeInit(GPIO_Reg_t *pGPIOx)
{
	            if(pGPIOx== GPIOA)
	              {
	        	     GPIOA_REG_RESET();
	              }
		  else  if(pGPIOx== GPIOB)
		          {
			            GPIOB_REG_RESET();
		          }

		  else  if(pGPIOx== GPIOC)
		          {
			            GPIOC_REG_RESET();
		          }
		  else  if(pGPIOx== GPIOD)
		          {
			            GPIOD_REG_RESET();
		          }
		  else  if(pGPIOx== GPIOE)
		          {
			            GPIOE_REG_RESET();
		          }
		  else  if(pGPIOx== GPIOH)
		          {
			            GPIOH_REG_RESET();
		          }

}










