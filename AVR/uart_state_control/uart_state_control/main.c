/*
 * uart_state_control.c
 *
 * Created: 04-01-2026 14:24:55
 * Author : hp
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <string.h>
#include "uart.h"

#define WHITE PC2
#define BLUE PC1
#define YELLOW PC0

volatile uint32_t count=0;

void timer_set_up()
{
	TCCR0A|=(1<<WGM01);// set CTC mode
	TCNT0=0;
	OCR0A=249;// count to 249, making 250 counts, compare match happens at 250*4us= 1 ms
	TCCR0B|=(1<<CS01)|(1<<CS00);// set prescalar 64, F_CPU= 250KHz, 1 pulse= 4us
	
}
void timer_interrupt_enable()
{
	TIMSK0|=(1<<OCIE0A);// interrupt enable
	
	sei();// enable global interrupt
}

ISR(TIMER0_COMPA_vect)
{
	count++;// increments each millisecond
}

uint32_t millis()
{
	uint32_t millis;
	
	cli();// disable interrupt
	millis=count;// read value
	sei();// resume interrupt
	      // SAFE READING
	return millis;
}
void WHITE_ON()
{
	PORTC|=(1<<WHITE);
	PORTC&=~((1<<BLUE)|(1<<YELLOW));// WHITE ON others off
}
void BLUE_ON()
{
	PORTC|=(1<<BLUE);
	PORTC&=~((1<<WHITE)|(1<<YELLOW));// BLUE ON others off
}
void YELLOW_ON()
{
	PORTC|=(1<<YELLOW);
	PORTC&=~((1<<WHITE)|(1<<BLUE));// BLUE ON others off
}
void blink_white(uint16_t time)
{
	static uint32_t prev_time=0;// initialize only once and keep value between function calls
	
	PORTC&=~((1<<BLUE)|(1<<YELLOW));// OFF others
	
	if(millis()-prev_time>time)
	{
		PORTC^=(1<<WHITE);// toggle
		prev_time=millis();
	}
}
void blink_blue(uint16_t time)
{
	static uint32_t prev_time=0;// initialize only once and keep value between function calls
	
	PORTC&=~((1<<WHITE)|(1<<YELLOW));// OFF others
	
	if(millis()-prev_time>time)
	{
		PORTC^=(1<<BLUE);// toggle
		prev_time=millis();
	}
}
void blink_yellow(uint16_t time)
{
	static uint32_t prev_time=0;// initialize only once and keep value between function calls
	
	PORTC&=~((1<<WHITE)|(1<<BLUE));// OFF others
	
	if(millis()-prev_time>time)
	{
		PORTC^=(1<<YELLOW);// toggle
		prev_time=millis();
	}
}
void blink_all(uint16_t time)
{
	static uint32_t prev_time=0;// initialize only once and keep value between function calls
	
	if(millis()-prev_time>time)
	{
		PORTC^=((1<<WHITE)|(1<<BLUE)|(1<<YELLOW));// toggle
		prev_time=millis();
	}
	
}
void all_off()
{
	
		PORTC&=~((1<<BLUE)|(1<<WHITE)|(1<<YELLOW));// off all
	
}
void all_on()
{
	
	PORTC|=((1<<BLUE)|(1<<WHITE)|(1<<YELLOW));// om all
	
}
uint8_t new_cmd_received()
{
	uint8_t status;
	status= UCSR0A&(1<<RXC0);
	if(status) return 1;// return 1 if new data/bye is arrived
	else
	return 0;
}
int main(void)
{
	DDRC|=(1<<WHITE)|(1<<BLUE)|(1<<YELLOW);// set LEDs as outputs
	
    uart_init();
	
	timer_set_up();
	
	timer_interrupt_enable();
	
	enum STATE {white,blue,yellow,w_blink,b_blink,y_blink,all_blink,off,on};// LED states
	enum STATE state=off;//  initialize the state as off
		
	char cmd[30];
		
		TX_string("Enter the state\n");
    while (1) 
    {
		
		
		if(new_cmd_received())// use RX_string() only if new data arrives,  it would block the entire main loop if there is no new command
		{
			RX_string(cmd,sizeof(cmd));
			TX_string("Current state : ");
			TX_string(cmd);
			TX_char('\n');
		}
		
		
		
		if(!strcmp(cmd,"WHITE"))// if command is WHITE
		{
			state=white;
		}
		else
		if(!strcmp(cmd,"BLUE"))// if command is BLUE
		{
			state=blue;
		}
		else
		if(!strcmp(cmd,"YELLOW"))// if command is BLINK
		{
			state=yellow;
		}
		else
		if(!strcmp(cmd,"BLINK WHITE"))// if command is BLINK WHITE
		{
			state=w_blink;
		}
		else
		if(!strcmp(cmd,"BLINK BLUE"))// if command is BLINK BLUE
		{
			state=b_blink;
		}
		else
		if(!strcmp(cmd,"BLINK YELLOW"))// if command is BLINK YELLOW
		{
			state=y_blink;
		}
		else
		if(!strcmp(cmd,"ALL OFF"))// if command is ALL OFF 
		{
				state=off;
		}
		else
		if(!strcmp(cmd,"ALL ON"))// if command is ALL ON
		{
			state=on;
		}
		else
		if(!strcmp(cmd,"ALL BLINK"))// if command is ALL BLINK
		{
			state=all_blink;
		}
			
			if(state==white)// if state is WHITE
			{
				WHITE_ON();// on white, off others
			}
			
			else
			if(state==blue)// if state is BLUE
			{
				BLUE_ON();// on blue, off others
			}
			else
			if(state==yellow)// if state is YELLOW
			{
				YELLOW_ON();// on blue, off other
			}
			else
			if(state==w_blink)// if state is BLINK WHITE
			{
				blink_white(300);// blink each 300ms
			}
			else
			if(state==b_blink)// if state is BLINK BLUE
			{
				blink_blue(300);// blink each 300ms
			}
			else
			if(state==y_blink)// if state is BLINK YELLOW
			{
				blink_yellow(300);// blink each 300ms
			}
			else
			if(state==off)// if state is OFF ALL
			{
				all_off();// off all
			}
			else
			if(state==on)// if state is ON ALL
			{
				all_on();// on all
			}
				else
				if(state==all_blink)// if state is ON ALL
				{
					blink_all(350);// blink all
				}
    }
}

