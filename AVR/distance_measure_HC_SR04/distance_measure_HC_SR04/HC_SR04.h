/*
 * HC_SR04.h
 *
 * Created: 02-02-2026 13:19:18
 *  Author: hp
 */ 

#include "uart.h"
#include "GPIO.h"

#ifndef HC_SR04_H_

                      // macros
#define trig PORTD2
#define echo PORTD3
#define buzzer PORTD4

			          // pin configuration structure
typedef struct  
{
	GPIO_Reg_t  *pGPIOx;
	uint8_t      trigger_pin;
	uint8_t      echo_pin;
	uint8_t      output_pin;
	
}HC_SR04_PinConfig_t;
					  							   
extern HC_SR04_PinConfig_t  HC_SR04_PinConfig; // General structure variable

                      // functions	
void HC_SR04_init();
void HC_SR04_send_trigger();
void HC_SR04_pin_init();
float HC_SR04_distance();
void set_buzzer();
void no_buzzer();					  			 



#endif /* HC_SR04_H_ */