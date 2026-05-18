

# UART Controlled LED Brightness System

## Objective
This project implements a UART-based control system for managing an LED’s ON/OFF state and brightness using PWM on an AVR microcontroller (ATmega328P).

The system receives commands from a serial terminal and responds in real-time.

---

## Features
- UART communication at **9600 baud**
- LED ON/OFF control via text commands
- PWM-based brightness control (0–100%)
- Real-time feedback via UART
- Modular design (UART + PWM drivers separated)

---

## Hardware Used
- ATmega328P
- LED connected to **PD6 (OC0A PWM pin)**
- Serial terminal (PuTTY )
- Bread board and jumper wires

---

## Timer & PWM Selection

### Timer Used: Timer0

- 8-bit timer
- Supports OC0A pin (PD6)
- Simple and efficient for LED brightness control

### PWM Mode: Fast PWM

- High frequency (~976 Hz with prescaler 64)
- No visible flicker
- Direct duty cycle control using OCR0A

---

## System Architecture

UART Terminal → UART Receive → Command Parser → PWM Controller → LED Output


---

## Command Format

| Command Type | Example | Action |
|--------------|--------|--------|
| ON           | ON      | LED fully ON (100% duty) |
| OFF          | OFF     | LED OFF (0% duty) |
| Brightness   | 0–100   | PWM duty cycle control |

---

## Module Overview

### 1. UART Module (`UART.c`)

Handles:
- UART initialization (9600 baud)
- Data transmission
- Data reception
- String handling
- Number transmission

Key functions:

- `Init_UART()`
- `UART_Send_data()`
- `UART_Receive_data()`
- `UART_Receive_string()`
- `UART_Send_string()`

---

### 2. PWM Module (`PWM.c`)

Handles:
- Fast PWM setup on Timer0
- Duty cycle control via OCR0A
- Command processing logic

Key functions:
- `Init_PWM()`
- `Update_LED_status()`

---

### 3. Main Application (`main.c`)

Flow:

1. Initialize UART
2. Initialize PWM
3. Continuously:
   - Receive UART command
   - Process command
   - Update LED state

---

## PWM Control Logic

### Brightness Conversion
```
compare value = (number * 255) / 100  
OCR0A = compare value
```

### ON/OFF Control
- ON → `OCR0A = 255`
- OFF → `OCR0A = 0`

---

## Important Technical Notes

### 1. PWM Overrides GPIO
Once Timer0 PWM is enabled:
- `PORTD6` cannot control LED anymore
- Only `OCR0A` controls output

### 2. UART Line Ending Handling
System supports:

- `\n`
- `\r`

To ensure compatibility with different terminals.

### 3. Buffer Safety

String reception prevents overflow:

- Max size enforced
- Null termination ensured

---

## Example UART Output
```
Input: 50  
Output: Brightness set to 50%

Input: ON  
Output: LED is now ON

Input: OFF  
Output: LED is now OFF

```
## Conclusion  
This project demonstrates:  
- UART communication handling  
- PWM generation using AVR timers  
- Embedded command parsing  
- Modular firmware design