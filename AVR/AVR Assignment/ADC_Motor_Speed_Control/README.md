


# DC Motor Speed Control Using ADC and PWM 

## Objective

This project regulates the rotational speed of a DC motor by mapping an analog input signal from a potentiometer to a PWM output signal.

The system uses:

- ADC to read potentiometer voltage
- Timer0 Fast PWM to generate PWM signal
- L293D motor driver to control DC motor speed

---

# Hardware Used

- ATmega328P
- Potentiometer
- DC Motor
- L293D Motor Driver Module
- External motor power supply
- Jumper wires

---

# Functional Overview

```text
Potentiometer → ADC Channel 0 → ADC Value Mapping → PWM Output → L293D → DC Motor
```

The potentiometer position controls:
- PWM duty cycle
- Motor speed

---

# PWM Configuration

## Timer Used
Timer0 (8-bit)

## PWM Mode
Fast PWM

## PWM Output Pin
OC0A → PD6

## PWM Frequency
For fast PWM,
```
PWM Frequency = F_CPU / ( Prescaler * (TOP+1) )
```
Where:
- `F_CPU = 16 MHz`
- Prescaler `N = 64`
- Top value `TOP = 255`

PWM frequency:

```text
≈ 976 Hz
```
This frequency is suitable for basic DC motor speed control.

---

# ADC Configuration

## ADC Channel
ADC0

## Reference Voltage
AVcc

## ADC Resolution

10-bit
ADC Range:

```text
0 → 1023
```

---

# ADC to PWM Mapping

## PWM Compare Register Range

Timer0 is an 8-bit timer:

```text
OCR0A range = 0 → 255
```


## Mapping Logic

ADC value is converted to 8-bit PWM value using:
```c
comp_value = ADC_Value >> 2;
```
Equivalent to:

```text
1023 / 4 ≈ 255
```
This converts:
- 10-bit ADC range (0–1023)
- into 8-bit PWM range (0–255)

for Timer0 OCR0A register.
This provides linear motor speed control.

## Example Code Snippet

### ADC to PWM Mapping

```c
uint16_t ADC_Value = Get_ADC_value();

uint8_t comp_value = ADC_Value >> 2;

OCR0A = comp_value;
```
---

# Project Structure

## 1. PWM Module (`PWM.c`)

Responsible for:
- Timer0 Fast PWM configuration
- PWM output generation on PD6

### Functions
- `Init_PWM()`

---

## 2. ADC Module (`ADC.c`)

Responsible for:
- ADC initialization
- Reading potentiometer voltage
- Mapping ADC value to PWM compare value

### Functions
- `Init_ADC()`
- `Get_ADC_value()`
- `Map_ADC_To_CompareValue()`

---

## 3. Main Application (`main.c`)

### Program Flow

1. Initialize PWM
2. Initialize ADC
3. Continuously:
   - Read potentiometer value
   - Convert ADC value to PWM compare value
   - Update OCR0A
   - Control motor speed

---

# L293D Motor Driver Connections

| L293D Pin | Connection |
|---|---|
| A+ / A- | DC Motor |
| EN1 | PD6 (OC0A PWM) |
| IN1 | 5V |
| IN2 | GND |
| VCC | 5V |
| VIN | External motor supply |
| GND | Common Ground |

---

## Important Notes

### 1. Common Ground Required
AVR ground and motor driver ground must be connected together.

### 2. External Motor Supply
The motor must be powered using VIN through an external supply.

Do NOT power the motor directly from the AVR board.

### 3. PWM Pin Behavior

Once PWM is enabled:
- PD6 is controlled by Timer0 hardware
- `PORTD6` can no longer directly control the pin

---

# Expected Behavior

| Potentiometer Position | Motor Speed |
|---|---|
| Minimum | Motor OFF |
| Mid Position | Medium Speed |
| Maximum | Full Speed |

---




---

# Conclusion

This project demonstrates:
- ADC interfacing
- PWM generation using AVR timers
- Motor speed control
- Analog-to-digital signal mapping
- Embedded modular firmware design