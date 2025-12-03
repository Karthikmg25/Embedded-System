/*
 * motor_speed_control_modularity.c
 *
 * Created: 03-12-2025 16:56:52
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include "dcmotor.h"
#include <util/delay.h>

int main(void)
{
    motor_init();// set up PWM
	
	speed_control(128);//  value between 0 and 255, control speed by setting duty cycle
	
    while (1) 
    {
		
		
    }
}

