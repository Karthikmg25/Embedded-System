


# Train Animation using GPIO

## Objective 
 
To create a train-like LED animation where a single active LED moves sequentially across multiple GPIO pins.
 
  
## Hardware Constraint 

On the Arduino Uno board (ATmega328P):  
  
- PB6 and PB7 are connected to the external 16 MHz crystal oscillator.  
- These pins are not available as general-purpose I/O.  
  
Therefore, only PB0–PB5 are usable from PORTB.  
  
To fulfill the requirement of 8 LEDs, PD6 and PD7 from PORTD were additionally used.  

  
  
## Hardware Used  

- AVR Microcontroller: ATmega328P (Arduino Uno board)  
- 8 LEDs connected to PD6, PD7, PB0-PB7 
- Breadboard and jumper wires


## Concepts Used

- GPIO configuration
- Sequential LED control using loops
- Hardware-aware pin mapping based on ATmega328P architecture
- Handling hardware constraints


## Working Principle

- All selected GPIO pins are configured as digital outputs using data direction registers.
- All LEDs are initially set to LOW state.
- The animation is implemented using two separate loops:
  - One loop controls LEDs connected to PORTB (PB0–PB5).
  - Another loop controls LEDs connected to PORTD (PD6–PD7).
- In each loop, a single LED is turned ON at a time in sequential order.
- A short delay is introduced to create a visible light effect.
- The active LED is turned OFF before moving to the next pin.
- This process runs continuously inside an infinite loop, creating a repeating train animation.
  
