# Multi Timer LED System

## Objective

Create two independent LED blinking tasks using  hardware timers and Compare Match interrupts.

-   Use **16-bit TIMER1** to toggle a **Status LED** every **2 seconds**
-   Use **8-bit TIMER2** to toggle a **Heartbeat LED** every **500 milliseconds**
-   Ensure both timers operate simultaneously without blocking each other


## Hardware Requirements

-   ATmega328P Microcontroller
-   2 LEDs
-   Breadboard and jumper wires

## Pin Configuration

|    Component    | Port Pin | Purpose 
| --------------   | -------- | ---------------------- |
| Status LED | PB1 | Blinks every 2 seconds |
| Heartbeat LED | PB2 | Blinks every 500ms|


## Concepts Used

-   AVR GPIO Configuration
-   Timer1 CTC Mode
-   Timer2 CTC Mode
-   Compare Match Interrupts
-   Interrupt Service Routine (ISR)
-   Hardware Timer Based Scheduling
-   Non-blocking Embedded Programming
- ----


## TIMER1 Configuration

### Purpose

Generate a 2-second LED toggle on PB1.

### Configuration


| Parameter        | Value           |
| ---------------- | --------------- |
| Timer            | TIMER1 (16-bit) |
| Mode             | CTC             |
| Prescaler        | 64              |
| OCR1A            | 49999           |
| Interrupt Period | 0.2 seconds     |


### Timing Calculation


`Time Period = 64×(49999+1)/16000000​ = 0.2 s`

The ISR runs every 0.2 seconds.

After 10 interrupts:

10 × 0.2 s = 2 s

The Status LED toggles every 2 seconds.

----

## TIMER2 Configuration

###  Purpose

Generate a 500 ms LED toggle on PB2.

###  Configuration

| Parameter | Value |
|---|---|
| Timer | TIMER2 (8-bit) |
| Mode | CTC |
| Prescaler | 256 |
| OCR2A | 249 |
| Interrupt Period | 4 ms |

### Timing Calculation

`Time Period = 256×(249+1)/16000000​ = 4 ms`
The ISR runs every 4 milliseconds.

After 125 interrupts:

125 × 4 ms = 500 milliseconds

The Heartbeat LED toggles every 500 milliseconds.

----
## Program Flow

1.  Configure PB1 and PB2 as output pins
2.  Initialize TIMER1 in CTC mode
3.  Initialize TIMER2 in CTC mode
4.  Enable Compare Match interrupts
5.  Timer ISRs independently manage LED toggling
6.  Main loop remains empty because timing is fully interrupt-driven

## Features

-   Independent timer operation
-   Accurate hardware-timed blinking
-   Interrupt-driven execution
-   No software delay functions
-   Non-blocking design
-   Simultaneous periodic task execution