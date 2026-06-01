

# STM32F401 I2C Driver 

A bare-metal I2C driver developed for the STM32F401RE microcontroller without using STM32 HAL or LL libraries. The driver provides polling-based I2C communication in controller (master) mode and supports both transmission and reception of data from I2C slave devices.

## Features

-   Bare-metal register-level implementation
    
-   Controller (Master) mode operation
    
-   Polling-based data transfer
    
-   7-bit slave addressing
    
-   Standard Mode (100 kHz)
    
-   Fast Mode (400 kHz)
    
-   Configurable Fast Mode duty cycle
    
-   Multi-byte transmission
    
-   Multi-byte reception
    
-   Repeated Start condition support
    
-   Compatible with STM32F401 I2C peripheral
    

## Supported Operations

### Initialization

Configures:

-   Peripheral clock
    
-   APB1 frequency settings
    
-   Clock Control Register (CCR)
    
-   Rise Time Register (TRISE)
    
-   Standard Mode and Fast Mode operation
    
-   Fast Mode duty cycle selection
    

### Data Transmission

The transmit API performs:

1.  START condition generation
    
2.  Slave address transmission (Write mode)
    
3.  Register address transmission
    
4.  Data buffer transmission
    
5.  Transfer completion wait
    
6.  STOP condition generation
    

### Data Reception

The receive API performs:

1.  START condition generation
    
2.  Slave address transmission (Write mode)
    
3.  Register address transmission
    
4.  Repeated START generation
    
5.  Slave address transmission (Read mode)
    
6.  Data reception
    
7.  STOP condition generation
    

The driver handles:

-   Single-byte reception
    
-   Two-byte reception
    
-   Multi-byte reception
    

according to the STM32 I2C peripheral reception sequence.


## GPIO Configuration

For STM32F401RE I2C1:

| Pin        | Function| Alternate Function|
|----------|-----------|------------------------|
| PB8      | I2C1_SCL  | AF4                    |
| PB9      | I2C1_SDA  | AF4                    |

Configuration requirements:

-   Alternate Function Mode
    
-   Open Drain Output Type
    
-  Pull-up Resistors (Internal/External)
    
-   AF4 Selection
    

## Driver APIs

### Peripheral Clock Control

```c
void I2C_PeriClockControl(I2C_Reg_t *pI2Cx, uint8_t EnorDi);
```
**Parameters**

-   `pI2Cx` : Pointer to the I2C peripheral (I2C1, I2C2, or I2C3)
-   `EnorDi` : Enable or disable peripheral clock

### Peripheral Initialization

```c
void I2C_Init(I2C_Handle_t* pI2C_Handle);

```
**Parameters**

-   `pI2C_Handle` : Pointer to I2C handle containing peripheral and configuration settings

### Buffer Transmission

```c
void I2C_Transmit_Buffer(
    I2C_Reg_t* I2Cx,
    uint8_t reg_address,
    uint8_t* Tx_Buff,
    uint8_t len,
    uint8_t slv_address
);

```
**Parameters**

-   `I2Cx` : I2C peripheral used for communication
-   `reg_address` : Register address inside the slave device
-   `Tx_Buff` : Pointer to data buffer to be transmitted
-   `len` : Number of bytes to transmit
-   `slv_address` : 7-bit slave device address

### Buffer Reception

```c
void I2C_Receive_Buffer(
    I2C_Reg_t* I2Cx,
    uint8_t reg_address,
    uint8_t* Rx_Buff,
    uint8_t len,
    uint8_t slv_address
);

```
**Parameters**

-   `I2Cx` : I2C peripheral used for communication
-   `reg_address` : Register address from which data is read
-   `Rx_Buff` : Pointer to buffer where received data is stored
-   `len` : Number of bytes to receive
-   `slv_address` : 7-bit slave device address

    

## Example Usage

```c
I2C_Handle_t I2C1Handle;

I2C1Handle.I2Cx = I2C1;
I2C1Handle.I2C_Config.I2C_Speed_mode = Standard_Mode;
I2C1Handle.I2C_Config.I2C_SCL_Speed = 100000;

I2C_Init(&I2C1Handle);

uint8_t tx_data = 0x55;

I2C_Transmit_Buffer(
    I2C1,
    0x10,
    &tx_data,
    1,
    0x68
);

```

## Limitations

-   Controller mode only
    
-   Polling-based operation only
    
-   No interrupt support
    
-   No DMA support
    
-   No 10-bit addressing
    
-   No multi-controller support
    

## Learning Objectives

This project was developed to gain practical experience with:

-   STM32 peripheral programming
    
-   I2C protocol operation
    
-   Register-level firmware development
    
-   Embedded C programming
    
-   Polling-based communication drivers
    
-   STM32F4 reference manual interpretation