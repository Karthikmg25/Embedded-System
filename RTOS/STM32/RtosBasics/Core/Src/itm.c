/*
 * itm.c
 *
 *  Created on: Jun 30, 2026
 *      Author: hp
 */

#include "itm.h"

void ITM_SendChar(uint8_t ch)
{
	 DEMCR |= (1 << 24);      // Enable trace
	 ITM_TRACE_EN |= (1 << 0); // Enable Stimulus Port 0
    while (!(ITM_STIMULUS_PORT0 & 1))
    {
        // Wait until FIFO has space
    }

    ITM_STIMULUS_PORT0 = ch;
}
