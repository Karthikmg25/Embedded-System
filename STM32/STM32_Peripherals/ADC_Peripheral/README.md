


# LDR Based LED Brightness Control using STM32 ADC

## Overview

This project demonstrates brightness control of an LED using an LDR (Light Dependent Resistor) and a custom STM32 ADC driver.

Ambient light intensity is measured through the ADC peripheral, and the ADC result is mapped into PWM duty cycle values to dynamically control LED brightness.

The project is designed mainly to demonstrate:
- Custom ADC driver implementation
- ADC interrupt handling
- Callback-based application logic
- PWM generation using timers
- Analog sensor interfacing using STM32

---

# Features

- ADC based analog input acquisition
- LDR sensor interfacing
- Timer PWM based LED brightness control
- Callback driven ADC conversion handling
- Modular driver architecture
- Baremetal STM32 peripheral programming

---

# Hardware Used

- STM32F401RE
- LDR
- 47k Resistor
- LED
- Breadboard & jumper wires

---

# Circuit Diagram

## LDR Voltage Divider

```text
GND
 |
[LDR]
 |
 +-----> ADC1 Channel 0 (PA0)
 |
[47k]
 |
VCC (3.3V)
```

## LED Output

```text
PWM Pin -----> LED -----> Resistor -----> GND
```

---

# Working Principle

The LDR forms a voltage divider circuit whose output voltage varies based on ambient light intensity.

- Bright environment → Lower ADC voltage
- Dark environment → Higher ADC voltage

The ADC continuously samples the analog voltage from the LDR circuit.

After every conversion:
1. ADC interrupt occurs
2. ISR invokes registered callback function
3. ADC result is mapped into PWM duty cycle
4. Timer PWM updates LED brightness

---

# Software Flow

```text
LDR
 ↓
ADC Conversion
 ↓
ADC ISR
 ↓
Callback Function
 ↓
ADC to PWM Mapping
 ↓
Timer PWM Update
 ↓
LED Brightness Control
```

---

# Callback Function

```c
void UpdateBrightness_From_ADC(uint16_t result)
{
    uint16_t duty = MapADC_To_PWM(result);

    Set_Brightness(duty);
}
```

The callback function is invoked automatically by the ADC ISR after every completed conversion.

---

# ADC Configuration

| Parameter | Configuration |
|---|---|
| ADC Resolution | 12-bit |
| Prescaler | Divide by 8 |
| Sample Time | 144 cycles |
| ADC Channel | Channel 0 (PA0) |

---

# PWM Control

Timer PWM is used to vary LED brightness.

ADC values are scaled into PWM duty cycle values using:

```text
ADC Value → PWM Duty Cycle
```

---

# Project Structure

```text
STM32_Peripherals/
│
├── ADC_Peripheral/
│   ├── Baremetal/
│   ├── Driver/
│   └── README.md
│
├── GPIO_Peripheral/
│
├── TIMER_Peripheral/
│
├── Application/
│   ├── Inc/
│   └── Src/
```

---

# Main Application Flow

```c
ADC_StartConversion(&handle, channel_0);

while(1)
{
    if(handle.ADC_Conversion_Complete)
    {
        ADC_StartConversion(&handle, channel_0);
    }
}
```

The ADC conversion process runs continuously while the callback function updates LED brightness dynamically.

---

# Learning Outcomes

This project demonstrates:
- ADC peripheral programming
- Timer PWM generation
- Interrupt handling
- Callback mechanism design
- Analog sensor interfacing
- Driver abstraction in embedded systems

---

# Future Improvements

- UART debugging support
- Moving average filtering for stable ADC readings
- Multi-channel ADC support
- Continuous conversion mode support

# Demo Video

This video demonstrates ADC based LDR brightness control using PWM on STM32F401RE.

[Watch Demo on YouTube](https://www.youtube.com/shorts/BdmMUiWDGA4)
