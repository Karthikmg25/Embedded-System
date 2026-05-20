

# BMP280 Temperature Sensor Interfacing using I2C (AVR)

## Overview
This project demonstrates how to interface the **BMP280 pressure & temperature sensor** with an **AVR microcontroller (ATmega328P)** using the I2C protocol. The program reads raw sensor data, applies calibration constants, and calculates accurate temperature values.

---

## Features
- I2C communication without external libraries
- Sensor ID verification
- Reads calibration constants from registers
- Configures BMP280 control register
- Reads 20-bit raw ADC temperature data
- Applies Bosch compensation algorithm
- Outputs temperature over UART

---


## Hardware Used
- ATmega328P (Arduino Uno compatible board)
- BMP280 sensor module
- PuTTY terminal (UART serial monitor over USB-to-Serial)

## Hardware Connections

| BMP280 Pin | ATmega328P Pin | Description |
|------------|----------------|-------------|
| VCC        | 3.3V    | Power supply |
| GND        | GND            | Ground |
| SDA        | PC4      | I2C Data line |
| SCL        | PC5     | I2C Clock line |
| CSB        | VCC     | Selects I2C mode (must be HIGH) |
| SDO        | GND or VCC | Sets I2C address (0x76 / 0x77) |
### Notes
- CSB must be HIGH for I2C mode
- Pull-up resistors (4.7kΩ) on SDA and SCL are required if not present on module
- Use 3.3V as Vcc for using bare BMP280 sensor 
---

## I2C Configuration

`SCL frequency = F_CPU / (16 + 2 × TWBR × Prescaler)`

- F_CPU = 16 MHz  
- Prescaler = 1  
- TWBR = 72  

So,

        SCL = 16,000,000 / (16 + 2 × 72 × 1)  
             = 16,000,000 / 160  
             = 100,000 Hz (100 kHz)

---

## BMP280 I2C Address
- `0x76` or `0x77` depending on SDO pin:
  - SDO = GND → `0x76`
  - SDO = VCC → `0x77`

---

## BMP280 Registers 

| Register | Function |
|----------|----------|
| 0xD0 | Chip ID |
| 0x88–0x8D | Calibration data |
| 0xF4 | Control measurement register |
| 0xFA–0xFC | Raw temperature data |

---

## Workflow

### 1. Sensor Initialization

- Initialize I2C and UART
- Read chip ID (should return `0x58` for BMP280)

---
### 2. Read Calibration Data

- Read 6 bytes from `0x88 - 0x8D (6 bytes)`
- Extract:
  - `dig_T1` (unsigned)
  - `dig_T2` (signed)
  - `dig_T3` (signed)

---

### 3. Configure Sensor

Set Control register value:  `0xF4 `  =   0x27

Settings:  
- Temperature oversampling = x1  
- Pressure oversampling = x1  
- Mode = Normal

### 4. Read 20-bit Raw ADC Value

The BMP280 outputs raw temperature or pressure data as a **20-bit value** spread across 3 registers:

| Register | Description |
|----------|-------------|
| 0xFA     | MSB (Most Significant Byte) |
| 0xFB     | LSB (Least Significant Byte) |
| 0xFC     | XLSB (Extra bits [3:0]) |


 20-bit ADC Combination Formula:

```c
ADC_value = (MSB << 12) |
            (LSB << 4)  |
            (XLSB >> 4);
```
### 5. Temperature Calculation

#### Compensation Formula:
```

var1 = ((((ADC >> 3) - (dig_T1 << 1))) * dig_T2) >> 11  
var2 = (((((ADC >> 4) - dig_T1)^2) >> 12) * dig_T3) >> 14  
t_fine = var1 + var2  
Temperature = (t_fine * 5 + 128) >> 8
```
## Final Output (UART Result)

```
Sensor ID of BMP280 : 0X58

Calibration values of BMP280

dig_t1 : 27707
dig_t2 : 26712
dig_t3 : -1000

raw ADC value : 544736

var1        : 165358
var2        : -599
t fine      : 164759

Temperature : 32.18

```