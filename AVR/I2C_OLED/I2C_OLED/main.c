#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "I2c.h"
#include "OLED.h"
#define OLED_ADD 0x3C

int main(void)
{
	
	
	I2c_init();// initialize i2c
	
	OLED_init();//  initialize OLED
	
	OLED_clear_display();
 
    initialise_OLED_buffer();
	
	OLED_print_string("count :",4,15);// string name, page, column
	OLED_print_string("OLED :",5,15);
	uint16_t num=0;
	while (1)
	{   
		                            // before printing new character,
		OLED_clear_space(4,65,10*5);// clear space for 10 characters from page6,column25
		
		OLED_print_num(num++,4,65);
		
		_delay_ms(1000);
	}                         
	
}