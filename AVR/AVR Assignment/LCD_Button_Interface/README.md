

# LCD Button Counter Interface

## Objective

Design a program using ATmega328P and a 16x2 LCD to display a 9-digit counter value.

- A short button press increments the counter by 1  
- A long button press (≥ 2 seconds) resets the counter to zero  

The program uses:
- Edge detection
- Button press duration measurement
- LCD interfacing in 4-bit mode  

---

## Hardware Requirements

| Component | Quantity |
|---|---|
| ATmega328P | 1 |
| 16x2 LCD Display | 1 |
| Push Button | 1 |
| 10k Potentiometer | 1 |
| Breadboard + Wires | Required |

---

## Pin Configuration

### LCD Connections

| LCD Pin | AVR Pin |
|---|---|
| RS | PB4 |
| EN | PB3 |
| D4 | PD4 |
| D5 | PD5 |
| D6 | PD6 |
| D7 | PD7 |
| RW | GND |
| VSS | GND |
| VDD | +5V |
| V0 | Potentiometer Output |

### Push Button

| Button Pin | AVR Pin |
|---|---|
| Button Output | PD2 |

- Internal pull-up resistor enabled  
- Active LOW logic used  

---

## Features

- 4-bit LCD interfacing  
- Edge detection for button press  
- Short press detection  
- Long press detection  
- 9-digit counter display  
- Counter reset after 2-second hold  
- Software timing using `_delay_ms()`  

---

## Program Logic

### Short Press

If button is pressed and released before 2 seconds:
- Counter increments by 1  
- Updated value displayed on LCD  

### Long Press

If button is held for 2 seconds or more:
- Counter resets to `000000000`  
- LCD display cleared and updated  

---

## Important Concepts Used

### Edge Detection

`if(previous_state == 1 && current_state == 0)`

Detects transition from released → pressed state.

### Press Duration Measurement

```c
while(!((PIND >> 2) & 1))
{
    press_count++;
    _delay_ms(1);
}
```
Measures how long the button is held (in milliseconds.

### Long Press Detection
```
if(press_count >= 2000)
```

Triggers reset when button is held for ≥ 2 seconds.

### Early Exit Using `break`

```
break;
```
Stops the timing loop immediately after long press is detected.

### Short Press Handling

```c
if(press_count < 2000)
{
    lcd_set_cursor(0,0);
    lcd_send_number(++count);
}
```
This block handles only valid short press events after loop exit.

-   `press_count < 2000` confirms it was a **short press**.
-   The counter is incremented.
-   The updated value is displayed on the LCD.


### Files

| File | Description |
| -------- | ------------------------- |
| `main.c` | Main application logic |
| `LCD.h` | LCD function declarations |
| `LCD.c` | LCD driver implementation 

## Expected Output

### Initial State
```
000000000
```

### After Short Presses
```
000000001
000000002
000000003
```

### After Long Press

```
000000000
```

## Key Learning Outcomes

-   LCD interfacing in 4-bit mode
-   GPIO input handling
-   Edge detection
-   Software debouncing concepts
-   Short vs long press handling