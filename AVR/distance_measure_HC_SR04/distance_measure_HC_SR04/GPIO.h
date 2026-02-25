/*
 * GPIO.h
 *
 * Created: 02-02-2026 14:05:31
 *  Author: hp
 */ 


#ifndef GPIO_H_
#define GPIO_H_

               // Register Definition Structure
typedef struct  
{
	volatile uint8_t PIN;
	volatile uint8_t DDR;
	volatile uint8_t PORT;
}GPIO_Reg_t;
               // GPIO pointers
			   
#define GPIOB   ((GPIO_Reg_t*)0x23) 			   			   
#define GPIOC   ((GPIO_Reg_t*)0x26)
#define GPIOD   ((GPIO_Reg_t*)0x29)

#endif /* GPIO_H_ */