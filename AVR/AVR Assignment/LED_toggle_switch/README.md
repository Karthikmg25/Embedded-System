# LED Toggle Using Push Button

## Objective
Interface with a Red LED, a Green LED, and a push-button switch. When the switch is pressed, the program must toggle the current active LED state.

## Hardware Used

- AVR Microcontroller: ATmega328P  
- Red LED connected to PD6  
- Green LED connected to PD5  
- Push Button connected to PD4  
- Breadboard and jumper wires


## Concepts Used  

- GPIO configuration  
- Digital Input and Output  
- Internal Pull-up Resistor  
- Bitwise Operations  
- Edge Detection  
- Software Debouncing
  

## Pin Configuration  
  
| Component | AVR Pin |  
|---|---|  
| Push Button | PD4 |  
| Green LED | PD5 |  
| Red LED | PD6 |


## Workflow  
  
1. Configure PD4 as input pin for push button.  
2. Enable internal pull-up resistor on PD4.  
3. Configure PD5 and PD6 as output pins.  
4. Initialize Green LED as ON and Red LED as OFF.  
5. Continuously read the button state.  
6. Detect falling edge (HIGH → LOW transition) to identify button press.  
7. Apply software debounce delay to avoid multiple triggers.  
8. Toggle the states of Red and Green LEDs.  
9. Repeat continuously.  
  

## Debouncing Logic  

Mechanical push buttons generate multiple rapid transitions due to bouncing.  
To avoid multiple unintended LED toggles from a single press:  
- Falling-edge detection is used.  
- A small delay is added after detecting the edge.


