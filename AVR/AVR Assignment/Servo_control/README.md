

# AVR Servo Control using ADC + Timer1 PWM

## Objective

This project demonstrates servo motor control using an AVR microcontroller by combining:
- ADC (potentiometer input)
- Timer1 PWM (servo signal generation)

The potentiometer position is read using ADC and mapped to a servo pulse width (1000µs–2000µs), which controls the servo angle from 0° to 180°.

---

## Features

- Reads analog input using 10-bit ADC
- Maps ADC value to servo pulse width
- Generates 50Hz PWM using Timer1
- Controls servo position in real time
- Uses modular ADC and PWM driver files

---

## Hardware Requirements

- ATmega328P microcontroller
- Servo motor (standard hobby servo)
- 10kΩ potentiometer
- 5V power supply
- Connecting wires

---

## Pin Configuration

| Signal | Pin | Description |
|--------|-----|-------------|
| ADC Input | ADC0 (PC0) | Potentiometer input |
| Servo PWM Output | OC1A (PB1) | Servo control signal |

---

## System Overview

1. Potentiometer outputs analog voltage (0–5V)
2. ADC converts it to digital value (0–1023)
3. Value is mapped to pulse width (1000–2000 µs)
4. Timer1 generates PWM signal with corresponding pulse width
5. Servo rotates based on pulse width

---

## ADC Configuration

- Reference Voltage: AVcc (5V)
- Channel: ADC0
- Prescaler: 128 (125 kHz ADC clock)
- Mode: Single conversion (polling)

### ADC to Pulse width  Formula

`Pulse Width = 1000 + ( ADC Value / 1023 ) * 1000`

---

## PWM Configuration (Timer1)

- Mode: Phase Correct PWM (Mode 10)
- Frequency: ~50Hz (20ms period)
- Prescaler: 8
- TOP: 19999

### Pulse Width Conversion


Timer tick = 0.5 µs (16 MHz / prescaler 8)
```
OCR1A = pulse_width / timer_tick
       = pulse_width / 0.5 µs
       = pulse_width × 2
       
   ```

---

## Servo Mapping

| Angle | Pulse Width | ADC Approx |
|------|-------------|-----------|
| 0° | 1000 µs | 0 |
| 90° | 1500 µs | ~512 |
| 180° | 2000 µs | 1023 |

---

## Code Structure

### `Init_PWM()`
- Configures Timer1 in PWM mode
- Sets 50Hz servo frequency
- Enables OC1A output

### `Init_ADC()`
- Initializes ADC in single-shot mode
- Sets AVcc reference
- Configures ADC0 channel

### `Get_ADC_value()`
- Starts ADC conversion
- Waits for completion
- Returns 10-bit result

### `Map_ADC_To_PulseWidth()`

- Converts ADC value into servo pulse width
- Ensures linear mapping from 0° to 180°

### `Set_pulse_width()`

- Converts pulse width into timer compare value
- Updates OCR1A register

---

## Working Principle

- Potentiometer position directly controls servo angle
- ADC reads input continuously in main loop
- PWM signal is updated in real time
- Servo smoothly follows potentiometer movement

---

## Summary

This project demonstrates a complete embedded control loop:
**Analog input → Digital conversion → Signal processing → PWM output → Mechanical motion**

