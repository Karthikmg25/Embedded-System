/*
 * HC_SR04.c
 *
 * Created: 02-02-2026 13:19:40
 *  Author: hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include "HC_SR04.h"
#include "GPIO.h"

HC_SR04_PinConfig_t   HC_SR04_PinConfig; // General structure variable

void HC_SR04_init()
{
	TCCR1A&=~((1<<WGM11)|(1<<WGM10));
	TCCR1B&=~((1<<WGM12)|(1<<WGM13));// set mode0 in Timer1, counts up to 65535
	
	TCCR1B|=(1<<CS10)|(1<<CS11);// set pre scalar 64, 1 count = 4us
	
}
void HC_SR04_send_trigger()
{
	_delay_us(2);// wait 2 us before trigger
	PORTD|=(1<<HC_SR04_PinConfig.trigger_pin);// set as HIGH for 10 us
	_delay_us(10);
	PORTD&=~(1<<HC_SR04_PinConfig.trigger_pin);// set low
	
}
void HC_SR04_pin_init()
{
	HC_SR04_PinConfig.pGPIOx->DDR|=(1<<HC_SR04_PinConfig.trigger_pin)|(1<<HC_SR04_PinConfig.output_pin);// set trig pin as output
	HC_SR04_PinConfig.pGPIOx->PORT &=~(1<<HC_SR04_PinConfig.trigger_pin);// default low
	HC_SR04_PinConfig.pGPIOx->DDR &=~(1<<HC_SR04_PinConfig.echo_pin);// set echo pin as input
}
float HC_SR04_distance()
{
	uint16_t echo_time;
	float distance;
	
	HC_SR04_send_trigger();// send trigger pulse
	
	while (!(HC_SR04_PinConfig.pGPIOx->PIN&(1<<HC_SR04_PinConfig.echo_pin)));// wait till echo goes HIGH
	
	TCNT1=0;// Reset timer
	
	while(HC_SR04_PinConfig.pGPIOx->PIN&(1<<HC_SR04_PinConfig.echo_pin));// wait till it goes LOW
	
	echo_time=TCNT1* 4;// time in micro sends
	
	distance=echo_time/58.0f;// distance in centimeters
	
	return distance;
	
}
void set_buzzer()
{
	GPIOD->PORT|=(1<<HC_SR04_PinConfig.output_pin);
}
void no_buzzer()
{
	GPIOD->PORT&=~(1<<HC_SR04_PinConfig.output_pin);
}
