/*
 * GPIO_Baremetal.c
 *
 *  Created on: May 22, 2026
 *      Author: hp
 */

#include "GPIO_Baremetal.h"

                                                    /* LED BLINK - GPIO Output mode */
void GPIO_Blink()
{
	// Enable clock for GPIOA
		GPIOA_CLK_EN();
	// Set PA0 as output pin
		GPIOA->MODER |=(1<< 0);
    // Toggle LED in a loop with delay:
	while(1)
	{
		GPIOA->ODR ^= (1<<0);
		for(volatile uint32_t i=0;i<100000;i++);
	}
}
                                                   /* BUTTON PRESS - GPIO Input mode */
void GPIO_ButtonPress()
{
	// Enable clock for GPIOA
		GPIOA_CLK_EN();

		// Set PA0 as output pin
		// - Set MODER1:0 = 01
		GPIOA->MODER |=(1<< 0);

		// Set PA1 as Input pin:
		// - Set MODER3:2 = 00
		GPIOA->MODER &=~(0x3 << 1*2);

		// Enable Pull-up resistor
		// - Avoids floating conditions and keeps default state as HIGH
		// - Set PUPDR3:2 = 01
		GPIOA->PUPDR |= (0x1<< 1*2);

		// Read Pin state using IDR register in a loop to detect button press
		// - Use De-bouncing logic to prevent multiple transitions per press
		// - De-bouncing logic:
		// -- detect falling edge
		// -- add stabilization delay for the bounce to settle
		// -- check if button is still pressed

		// variables for edge detection:
		uint8_t current_state, previous_state=1;
		while(1)
		{
			// Read current button state
			current_state = (GPIOA->IDR>> 1)&1;

			// If button press happens: falling edge
			if(previous_state == 1 && current_state==0)
			{
				// Apply delay for bounce to settle: Bad design, better sue timer/systick based delays for accuracy
				CRUDE_DELAY(20000);
				// check if button still pressed
				if(((GPIOA->IDR>> 1)&1) == 0)
				{
					// Perform toggle operation: PA0
					GPIOA->ODR ^= (1<< 0);
				}


			}
			// Reset Previous state
			previous_state= current_state;

		}

}
                                                              /* BUTTON PRESS - External Interrupt */
void External_Interrupt()
{

	   // Step 1) Map GPIO pin with External interrupt line:
	   // - Use EXTICR register in  SYSCFG peripheral
	   // - Map PA1 with EXTI1
	   // = Push port code of GPIOA (0000) to EXTICR[0] (register for EXTI3-EXTI0)

	    // enable clock
	    SYSCFG_CLK_EN();
	    // set port code after clearing bits
	    SYSCFG->EXTICR[0]&=~(0xF<< 1*4);
	    SYSCFG->EXTICR[0] |=(0x0<< 1*4);


	    // Step 2) Enable external interrupt & Set mode (Falling/Rising)
	    // - Use IMR register in  EXTI peripheral
	    // - Lower 16 bits (IMR15:0) controls 16 EXTI lines
	    // - Use FTSR for Interrupt on falling edge
	    // - Use RTSR for Interrupt on rising edge
	    // - This generates interrupt request and sets pending flag

	    EXTI->IMR |= (1<< 1 );
	    EXTI->FTSR |= (1<< 1);
	    // clear RTSR
	    EXTI->RTSR&=~(1<<1);


	    // Step 3) Enable interrupt on NVIC and set priority(Optional)
	    // - Use ISER register in  EXTI peripheral
	    // - One ISER register controls 32 interrupts (Total 8 ISER registers)
	    // - EXTI1 interrupt line is mapped to IRQ number 7 (ISER[0])
	    // - This enables CPU to respond to interrupt request and jump to interrupt vector

	    NVIC->ISER[0] |= (1<< 7);

	    // Step 4) GPIO Configuration for the pin PA1
	    // - Set the pin as input
	    // - Enable pull up to detect falling edge

	    GPIOA_CLK_EN();
	    GPIOA->MODER &=~(0x3 << 1*2);
	    GPIOA->PUPDR &=~(0x3 << 1*2);
	    GPIOA->PUPDR |= (01 << 1*2);

	    // Set LED pin- PA0 as output
	    GPIOA->MODER |= (1<< 0);
}
// Step 5) Write Interrupt service routine to toggle LED
// - Clear pending bit inside the ISR
// - Lower 16 bits of Pending register stores pending bits
void EXTI1_IRQHandler()
{
	// Clear by writing 1
	EXTI->PR |= (1<< 1);
    // LED toggle
	GPIOA->ODR ^= (1<< 0);
}








