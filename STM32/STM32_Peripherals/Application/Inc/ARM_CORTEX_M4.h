/*
 * ARM_CORTEX_M4.h
 *
 *  Created on: May 22, 2026
 *      Author: hp
 */

#ifndef ARM_CORTEX_M4_H_
#define ARM_CORTEX_M4_H_

/* ARM CORE PERIPHERALS*/
/**********************
 * - NVIC
 * - SysTick
 */

/*
 *   NVIC
 *
 *   - Register definition
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
	volatile uint8_t  IP[240];


}NVIC_Reg_t;
#define NVIC ((NVIC_Reg_t*)(0xE000E100))




/*
 *   SysTick
 *
 *   - Register definition
 */
typedef struct
{
	volatile uint32_t CTRL;
	volatile uint32_t LOAD;
	volatile uint32_t VAL;
	volatile uint32_t CALIB;

}SYSTICK_Reg_t;
#define SYSTICK   ((SYSTICK_Reg_t*)0xE000E010)
/*
 * Important bits in SYSTICK
 */
#define SYSTICK_CTRL_ENABLE       0
#define SYSTICK_CTRL_TICKINT      1
#define SYSTICK_CTRL_CLKSOURCE    2
#define SYSTICK_CTRL_COUNTFLAG    16



#endif /* ARM_CORTEX_M4_H_ */
