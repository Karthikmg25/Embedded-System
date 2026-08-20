/*
 * itm.h
 *
 *  Created on: Jun 30, 2026
 *      Author: hp
 */

#ifndef SRC_ITM_H_
#define SRC_ITM_H_
#include <stdio.h>
#include <stdint.h>
// ITM and SerialWireVeiwer Hardware for debugging

/*Instead of sending characters over a UART, you write them into the ITM,
 *  which forwards them to the debugger through the SWO pin*/
// key registers

//Debug Exception and Monitor Control Register : Enables or Disables debug hardware
#define DEMCR (*(volatile uint32_t *)0xE000EDFC)
//ITM Trace Enable Register (TER): To enable ports (hardware has 32 ports, port0 usually used for printf)
#define ITM_TRACE_EN (*(volatile uint32_t *)0xE0000E00)
// when data is written to port0, its then moved to a FIFO/queue for transmission to debugger
#define ITM_STIMULUS_PORT0 (*(volatile uint32_t *)0xE0000000)

void ITM_SendChar(uint8_t ch);



#endif /* SRC_ITM_H_ */
