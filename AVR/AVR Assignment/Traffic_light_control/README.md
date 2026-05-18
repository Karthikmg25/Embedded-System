
# Traffic Light Controller using custom delay function

## Objective

This project simulates a basic one-way traffic light controller using three LEDs connected to PORTB of Atmega328P. The program controls Green, Orange, and Red LEDs in a continuous traffic light sequence without using the inbuilt `_delay_ms()` function.

## Requirements

-   AVR Microcontroller (ATmega328P)
-   3 LEDs (Green, Orange, Red)
-   Breadboard and jumper wires

## GPIO Connections

| LED Color   | GPIO Pin |
|------------|----------|
| Green LED  | PORTB0   |
| Orange LED | PORTB1   |
| Red LED    | PORTB2   |

## Working Principle

The traffic light sequence works as follows:

1.  Green LED turns ON for 10 seconds
2.  Green LED turns OFF and Orange LED turns ON for 3 seconds
3.  Orange LED turns OFF and Red LED turns ON for 10 seconds
4.  Sequence repeats continuously
----------

## Timer Configuration

TIMER0 is configured in CTC (Clear Timer on Compare Match) mode to generate a delay without using `_delay_ms()`.

### Timer Settings

-   CPU Frequency: 16 MHz
-   Prescaler: 64
-   OCR0A Value as top : 249
-   Compare Match Interval: 1 ms

Timer frequency equation:

f_timer =  F_CPU / ( prescaler * (1 + OCR0A) )

Using:

-   `F_CPU = 16 MHz`
-   `Prescaler = 64`
-   `OCR0A = 249`

A compare match event is generated every 1 millisecond.

The delay function repeatedly waits for 1000 compare match events to create a 1 second delay.


## Features

-   Uses hardware TIMER0 instead of software delay loops
-   Modular code structure using functions
-   Uses macros for LED readability
-   Continuous traffic light operation


## Functions Used

| Function         | Description                          |
|-----------------|--------------------------------------|
| `Init_timer()`    | Configures TIMER0 in CTC mode        |
| `delay_s()`       | Creates delay using compare match polling |
| `Init_pins()`     | Configures LED pins as outputs       |
| `ON_GREEN_LED()`  | Turns ON Green LED                   |
| `ON_ORANGE_LED()` | Turns ON Orange LED                  |
| `ON_RED_LED()`    | Turns ON Red LED                     |