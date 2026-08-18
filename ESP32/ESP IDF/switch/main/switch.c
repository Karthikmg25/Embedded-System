#include <stdio.h>
#include "driver/gpio.h"


#define LED_GPIO    2
#define BUTTON_GPIO 4
void app_main(void)
{
     gpio_reset_pin(LED_GPIO);
     gpio_reset_pin(BUTTON_GPIO);
     
     gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT); // set LED as output
      gpio_set_level(LED_GPIO,1);
     gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);// set button as input
     gpio_pullup_en(BUTTON_GPIO);// enable pullup
     
     while(1)
     {
          if(!(gpio_get_level(BUTTON_GPIO))) // button pressed, BUTTON_GPIO=0
          {
               gpio_set_level(LED_GPIO,1);
          }
          else                               // button idle , BUTTON_GPIO=1
          {
                gpio_set_level(LED_GPIO,0);
          }
          
     }
     

}
