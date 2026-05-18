/*
 * traffic_control.h
 *
 * Created: 15-05-2026 20:29:31
 *  Author: hp
 */ 


#ifndef TRAFFIC_CONTROL_H_
#define TRAFFIC_CONTROL_H_

#include <avr/interrupt.h>

// Use Macros for readability:
#define GREEN_LED  PORTB0
#define ORANGE_LED PORTB1
#define RED_LED    PORTB2

// Flag set in ISR, shared across all files:
extern volatile uint8_t flag;

void Init_timer();
void delay_s(uint8_t sec);
void Init_pins();
void ON_GREEN_LED();
void ON_ORANGE_LED();
void ON_RED_LED();
void Init_interrupt();
void check_flag();

#endif /* TRAFFIC_CONTROL_H_ */