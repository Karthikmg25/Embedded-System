# STM32 Distance Measurement System using HC-SR04

# Overview

This project implements a real-time distance measurement system using the HC-SR04 ultrasonic sensor interfaced with an STM32 microcontroller. The primary focus is on bare-metal driver development and precise timing using general-purpose timers.

Custom drivers for GPIO and USART were developed from scratch, and general-purpose timers were configured to achieve microsecond-level resolution required for accurate distance calculation.



# Objectives

* Using custom GPIO driver for input/output operations
* Using USART driver for serial communication
* Configure general-purpose timers for precise time measurement
* Generate microsecond-level delays using SysTick
* Interface HC-SR04 sensor safely with STM32
* Perform real-time distance calculation and transmission



# Components Used

* STM32 Nucleo Board (STM32F401)
* HC-SR04 Ultrasonic Sensor
* Resistors (for voltage divider)
* Breadboard and jumper wires
* STM32CubeIDE


# Working Principle

The HC-SR04 sensor operates using ultrasonic wave reflection:

1) A 10µs trigger pulse is generated via GPIO.
2) The sensor emits ultrasonic waves.
3) Reflected waves from an object are received back.
4) The Echo pin stays HIGH for the duration of the round trip.
5) Timer captures this pulse width.

Distance is calculated using:

Distance = (Time × Speed of Sound) / 2

# System Architecture

## GPIO Driver

* Trigger pin configured as output
* Echo pin configured as input
* Direct register-level configuration
* Supports pin read/write operations

## USART Driver

* Custom implementation for serial communication
* Polling-based transmission
* Used for real-time monitoring of measured distance

## Timer Configuration (TIM2)

* Configured for 1µs resolution
* Used to measure Echo pulse width accurately
* Direct register manipulation 

## SysTick Timer

* Used for generating precise microsecond delays
* For 10µs trigger pulse generation



# Implementation Flow

* Initialize GPIO, USART, and Timer peripherals
* Generate 10µs trigger pulse using SysTick
* Wait for Echo pin to go HIGH
* Start timer (TIM2)
* Wait for Echo pin to go LOW
* Stop timer and capture count
* Convert time into distance
* Transmit result via USART

# Hardware Considerations

* STM32 GPIO operates at 3.3V
* HC-SR04 Echo outputs 5V
* Implemented a voltage divider to safely step down 5V → 3.3V

# Features

* Custom GPIO driver implementation
* Custom USART driver
* TIM2 configured for microsecond resolution
* SysTick-based delay generation
* Real-time distance measurement
* USART output for monitoring
* Safe 5V to 3.3V interfacing

# Key Observations

* Verified system clock configuration using RCC->CFGR register
    - Checked SWS, HPRE, and PPRE1 bits
    - Confirmed SYSCLK = 16 MHz (HSI)
    - Ensured proper APB1 clock configuration for TIM2
* Floating-point operations introduced timing inconsistencies
    - FPU was not enabled
    - Switching to integer-based calculations resolved the issue
* Polling-based implementation showed limitations
    - CPU remains blocked during echo measurement
    - Not ideal for time-critical or multitasking systems


# Results   

* Achieved stable and accurate distance measurements
* Verified readings using real-world scale comparison
* Continuous data transmission via USART for monitoring

Example Output:  
Distance: 15 cm  
Distance: 18 cm  
Distance: 20 cm  

# Key Learnings

* Designing and implementing low-level peripheral drivers
* Understanding STM32 clock system and timer configuration
* Achieving microsecond-level timing precision
* Debugging hardware-software interaction step by step
* Importance of efficient data types in embedded systems

# 💻 Source Code

The full driver implementations are available in the repository. The snippet below shows the main application logic and how the drivers are integrated.


## Application Layer (main.c)

```
#include <stdint.h>
#include <stdio.h>
#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_usart_driver.h"
#include "HCSR04_Driver.h"

int main(void)
{
	usart2_GPIO_PinSetUp();                         /* SET PA2 AND PA3 AS TX & RX of USART2*/

	USART_Handle_t usart2;
	usart2.pUSARTx=                         USART2;             /* SELECTT USART CONFIGURATIONS */
	usart2.USART_Config.USART_Baud=         USART_STD_BAUD_9600;
	usart2.USART_Config.USART_Mode=         USART_MODE_RXTX ;
	usart2.USART_Config.USART_Parity=       USART_NO_PARITY ;
	usart2.USART_Config.USART_StopBit=      USART_STOP_BIT_1;
	usart2.USART_Config.USART_WordLength=   USART_WORDLEN_8BITS;

	USART_Init(&usart2);                            /* INITIALISE USART WITH CONFIGURATIONS       */
	USART_PeripheralControl(USART2, ENABLE);        /* ENABLE USART                               */

	SysTick_Delay_init();                           /* INITIALISE SYSTICK FOR MICRO-SECONDS DELAY */
	HCSR04_GPIO_setup();                            /* INITIALISE GPIO SETUP FOR SENSOR PINS      */
	Timer2_Init();                                  /* INITIALISE TIMER2 FOR MEASUTING ECHO TIME  */

	int distance;
	char string[40];
	while(1)
	{
		distance = HCSR04_GetDistance();            /* MEASURE DISTANCE                            */

		if(distance<10)
		{                                           /* TURN ON BUZZER IF DISTANCE IS LESS THAN 10CM*/
			Buzzer_ON();
		}
		else
		{
			Buzzer_OFF();
		}
		sprintf(string, "Distance : %02d cm",distance); /* SEND AS STRING */
		USART_Transmit_String(USART2, string);           
                                                   /* PRINT DISTANCE ON SERIAL MONITOR VIA USART   */
		USART_Transmit(USART2,'\n');
		Delay_us(50000);
	}
}

```


**Driver implementations available at**: (https://github.com/Karthikmg25/Embedded-System/tree/main/STM32/driver_dev/drivers)

## Key Modules
* GPIO Driver    – Custom register-level implementation for pin configuration and I/O operations
* USART Driver   – Polling-based transmission using status flags 
* HC-SR04 Driver – Handles trigger generation, echo timing using TIM2, and distance calculation

## 🎥 Demo Video

**Watch here**: <https://www.youtube.com/watch?v=K9l9h-CRrJU>