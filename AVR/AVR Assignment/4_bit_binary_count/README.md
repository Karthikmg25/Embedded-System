

# 4-bit Binary Counter

## Objective:

 Write a program that uses four LEDs to display a 4-bit binary count triggered by a switch.


 ## Hardware  Used 

- AVR Microcontroller: ATmega328P 
-   4 × LEDs
-   1 × Push button switch
-   Breadboard and connecting wires


## Concepts Used

-   Edge-triggered input handling
-   Software debouncing (simple and effective)
-   Modular design using `Update_count()` function
-   Direct bit manipulation for fast GPIO control
-   Wrap-around counter (0 → 15 → 0)


 ##  Working Principle

-   The push button is configured with an **internal pull-up resistor**.
-   Button press is detected using **falling edge detection** (HIGH → LOW transition).
-   Simple **debouncing logic** is implemented:
    -   20 ms delay after detecting edge
    -   Re-checks button state for confirmation
-   A global counter variable stores the current count (0–15).
-   The LED output is directly updated using the lower nibble of PORTB.
-   Lower 4 bits of count (bit0- bit3) is written to PORTB.