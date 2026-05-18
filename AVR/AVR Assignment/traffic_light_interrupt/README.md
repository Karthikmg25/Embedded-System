

# Traffic Light Controller with External Interrupt

## Objective

This project implements a one-way traffic light controller using an AVR microcontroller and three LEDs connected to PORTB. The system operates using hardware TIMER0 without using the inbuilt `_delay()` function. An external interrupt is also implemented to simulate an emergency traffic priority request.


## Features

-   Hardware timer based delay using TIMER0
-   CTC (Clear Timer on Compare Match) mode
-   External Interrupt (INT0) support
-   Interrupt flag handling using ISR
-   Continuous traffic light sequence
-   Emergency Green light priority mode
-   Modular embedded C programming structure
- 
## Hardware Requirements

-   ATmega328P Microcontroller
-   3 LEDs
-   Push button


## GPIO Connections

| Component          | GPIO Pin |
|-------------------|-----------|
| Green LED         | PORTB0    |
| Orange LED        | PORTB1    |
| Red LED           | PORTB2    |
| Push Button (INT0)| PD2       |

## Normal Traffic Light Sequence

| State      | Duration   |
|------------|------------|
| Green LED  | 10 seconds |
| Orange LED | 3 seconds  |
| Red LED    | 10 seconds |

## External Interrupt Operation

The push button is connected to the INT0 pin (PD2) and configured for falling edge triggering.

When the button is pressed:

1.  The ISR does not immediately change traffic states
2.  The ISR only sets an interrupt flag
3.  The main program checks the flag after completion of the current light duration
4.  If the flag is set:
    -   Green LED is prioritized for a fixed duration
    -   The flag is cleared
    -   Normal traffic sequence resumes

This design avoids sudden state transitions inside the ISR and follows safer embedded interrupt handling practices.

## TIMER0 Configuration

TIMER0 is configured in CTC mode to generate a compare match every 1 millisecond.

### Timer Settings

-   CPU Frequency: 16 MHz
-   Prescaler: 64
-   OCR0A Value: 249

Timer frequency equation:

```
f_timer = F_CPU / (prescaler * (1 + OCR0A))
```

Using:

-   `F_CPU = 16 MHz`
-   `Prescaler = 64`
-   `OCR0A = 249`

This generates a compare match every 1 ms.

## Project Structure

| File                | Description                                       |
|---------------------|---------------------------------------------------|
| `main.c`            | Main traffic light sequence logic                 |
| `traffic_control.c` | Timer, GPIO, interrupt, and LED                          control functions |
| `traffic_control.h` | Function declarations and macros                


## Functions Used

| Function           | Description                               |
|-------------------|-------------------------------------------|
| `Init_timer()`     | Configures TIMER0 in CTC mode             |
| `delay_s()`        | Creates delay using compare match polling |
| `Init_pins()`      | Configures GPIO pins                      |
| `Init_interrupt()` | Configures INT0 external interrupt        |
| `ON_GREEN_LED()`   | Turns ON Green LED                        |
| `ON_ORANGE_LED()`  | Turns ON Orange LED                       |
| `ON_RED_LED()`     | Turns ON Red LED                          |
| `check_flag()`     | Handles emergency Green priority mode     

## Interrupt Handling

### ISR Used

```
ISR(INT0_vect)
```

### ISR Behavior

-   Sets a global volatile flag
-   Main program handles traffic state changes

This keeps the ISR short and non-blocking.