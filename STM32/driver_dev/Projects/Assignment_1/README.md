# STM32 GPIO Tasks – Toggle & Alternate LED Blinking

## Overview

This project demonstrates basic GPIO driver usage on STM32 through two tasks implemented in a single `main.c` file.  
**Task 1**: Toggle onboard LED using onboard button  
**Task 2**: Alternate blinking of two external LEDs  

## Program Logic

### Task 1 – Onboard LED Toggle Using Button (PC13)

* The onboard button (`PC13`) is configured as an **input pin**
* The onboard LED (`PA5`) is configured as an **output pin**
* Button handling is implemented using **edge detection**, not level detection  
Two variables are used:  
* `prev_state` – stores the previous button state
* `curr_state` – stores the current button state  
The toggle operation is performed only when a falling edge is detected  
```c
prev_state = HIGH
curr_state = LOW
```
* By detecting a **HIGH → LOW transition**, the LED toggles **exactly once per press**  
* Reading an edge helps to prevent **multiple toggles during a single press**

### Task 2 – Alternate Blinking of External LEDs (PA0 & PA1)

* Two external LEDs are connected to: `PA0` and `PA1`  
* Both pins are configured as GPIO outputs  
* `PA0` is set as HIGH and `PA1` is set as LOW initially
* Instead of using blocking delays that halt CPU execution, a **software counter** is used to control blinking timing  
```c
uint32_t count=0, time=0;
```
* `count` variable increments continuously inside the main loop
* Both LEDs toggle their states when `count - time` reaches a predefined threshold  
* `time` variable is updated with the current `count` value after completing one full blink cycle

## Pin Mapping

### Task 1 – Onboard Button & LED
 
| Component           | Port                     | Pin       | Mode          |
|---------------------|--------------------------|-----------|---------------|
| Onboard Button      | PC                       | 13        | Input         | 
| Onboard LED         | PA                       | 5         | Output        |

### Task 2 – External LEDs

| LED                 | Port                     | Pin       | Mode          |
|---------------------|--------------------------|-----------|---------------|
| LED1                | PA                       | 0         | Output        | 
| LED2                | PA                       | 1         | Output        |

## Wiring Diagram (Task 2)

```c
PA0 --------|>|---- GND
PA1 --------|>|---- GND
```
* LED anode connected to GPIO pin  
* LED cathode connected to GND

## Observations

* Button toggling works reliably with state retention  
* LED toggles only once per button press (no multiple toggles due to bouncing)  
* Alternate LED blinking on PA0 and PA1 is stable and consistent  
* No unexpected behavior during continuous operation
* GPIO driver abstraction improves readability and reuse

## 💻 Source Code

```c
#include <stdint.h>
#include "stm32f401re_gpio_driver.h"

int main(void)
{
	GPIO_Handle_t GPIOled;// declare structure variable for handle structure
	GPIO_Handle_t GPIOread;// for reading button press

	GPIOled.pGPIOx=GPIOA;
	GPIOled.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_5;   // set required configurations for PA5
	GPIOled.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
	GPIOled.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
	GPIOled.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;
	GPIOled.GPIO_PinConfig.GPIO_PinPUPD=GPIO_NO_PUPD;

	GPIO_Init(&GPIOled);// initialize PA5

	GPIOread.pGPIOx=GPIOC;                                  // set configurations for PC13
	GPIOread.GPIO_PinConfig.GPIO_PinNumber=GPIO_PIN_NO_13;  // button is active low
	GPIOread.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_IN ;     // configure pin PC13, on-board button
	GPIOread.GPIO_PinConfig.GPIO_PinPUPD = GPIO_PIN_PULLUP; // internal pull-up


	GPIO_Init(&GPIOread);// initialize PC13

	GPIO_Handle_t led1, led2;// external LED blink

	led1.pGPIOx= GPIOA;
	led2.pGPIOx= GPIOA;
	led1.GPIO_PinConfig.GPIO_PinNumber= GPIO_PIN_NO_0;
	led2.GPIO_PinConfig.GPIO_PinNumber= GPIO_PIN_NO_1;
    led1.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
    led2.GPIO_PinConfig.GPIO_PinMode=GPIO_MODE_OUT;
    led1.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    led2.GPIO_PinConfig.GPIO_PinSpeed=GPIO_SPEED_FAST;
    led1.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;
    led2.GPIO_PinConfig.GPIO_PinOPType=GPIO_OP_TYPE_PUSHPULL;// set configurations for led1 and led2

    GPIO_Init(&led1);
    GPIO_Init(&led2);// initialize PA0 and PA1

    GPIO_Write_OutputPin(GPIOA, GPIO_PIN_NO_0, 1);
    GPIO_Write_OutputPin(GPIOA, GPIO_PIN_NO_1, 0);// set PA0 as HIGH and PA1 as LOW at first

    uint8_t current_state=1, prev_state=1; // to detect falling edge
    uint32_t count=0, time=0;              // to set up a software counter

	while(1)
	{
		current_state=GPIO_Read_InputPin(GPIOC, GPIO_PIN_NO_13);

		if(prev_state==1 && current_state==0)// button press creates falling edge
		{
			GPIO_Toggle_OutputPin(GPIOA ,GPIO_PIN_NO_5);// toggle LED
		}
		prev_state= current_state;// update previous state

		if(count-time > 200000)// non-blocking delay
		{
			GPIO_Toggle_OutputPin(GPIOA, GPIO_PIN_NO_0 );
			GPIO_Toggle_OutputPin(GPIOA, GPIO_PIN_NO_1 );
			time=count;// update time
		}
         count++;
	}

}
```

## 🎥 Demo Video

**Watch here**: <https://www.youtube.com/watch?v=TfJVR7zAKJw>


