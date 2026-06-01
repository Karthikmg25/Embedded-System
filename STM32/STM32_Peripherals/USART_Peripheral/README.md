


# STM32F401RE USART Driver

## Overview

This project implements a custom USART (Universal Synchronous/Asynchronous Receiver Transmitter) driver for the STM32F401RE microcontroller without relying on vendor HAL libraries.

The driver provides low-level register-based APIs for configuring and operating USART peripherals and serves as a learning-oriented implementation of serial communication on ARM Cortex-M microcontrollers.

Supported peripherals:

-   USART1
    
-   USART2
    
-   USART6
    

----------

## Features

### Peripheral Management

-   Enable/Disable USART peripheral clocks
    
-   Enable/Disable USART peripherals
    

### Configuration

-   Baud rate configuration
    
-   8-bit and 9-bit word lengths
    
-   Oversampling by 8 or 16
    
-   RX only mode
    
-   TX only mode
    
-   Full duplex TX/RX mode
    

### Parity Support

-   No parity    
-   Even parity   
-   Odd parity
    

### Stop Bit Configuration

-   1 stop bit    
-   0.5 stop bit    
-   2 stop bits   
-   1.5 stop bits
    

### Hardware Flow Control

-   CTS   
-   RTS   
-   CTS + RTS   
-   No flow control
    

### Data Transfer APIs

-   Single frame transmit
    
-   Single frame receive
    
-   Buffer transmit
    
-   Buffer receive
    
-   String transmit
    
-   String receive
    

----------

## Driver Design

The driver follows a layered design approach.

### 1. Clock Control Layer

Responsible for enabling and disabling clocks required by USART peripherals.

Example API:

```c
USART_PeriClockControl(USART2, ENABLE);

```

----------

### 2. Peripheral Configuration Layer

Configures USART operating parameters such as:

-   Baud rate   
-   Word length    
-   Parity    
-   Stop bits   
-   Oversampling    
-   Hardware flow control
    

Example:

```c
USART_Handle_t handle;

handle.pUSARTx = USART2;
handle.USART_Config.USART_Baud = 9600;
handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;
handle.USART_Config.USART_OverSampling = USART_OVERSAMPLING_16;
handle.USART_Config.USART_Mode = USART_MODE_RXTX;

USART_Init(&handle);

```

----------

### 3. Data Transfer Layer

Provides blocking APIs for transmitting and receiving data.

Examples:

```c
USART_Transmit();
USART_Recieve();

USART_Transmit_Buffer();
USART_Receive_Buffer();

USART_Transmit_String();
USART_Receive_String();

```

----------

## Baud Rate Generation

The driver calculates the BRR register value automatically based on:

-   Peripheral clock frequency
    
-   Baud rate
    
-   Oversampling mode
    

Supported modes:

-   Oversampling by 16
    
-   Oversampling by 8
    

The implementation performs integer arithmetic to avoid floating-point operations.

----------

## Example Application

### USART Echo Terminal

The demo application uses USART2 connected to a PC through a USB-to-UART interface.
Pins used:

| Signal |STM32 Pin |  
|---|-----|  
|TX | PA2 |   
| RX | PA3 |   

Configuration:

-   Baud Rate: 9600
-   Word Length: 8 bits    
-   No parity    
-   Full duplex TX/RX    
-   Oversampling by 16
    

----------

### Startup Message

After initialization the MCU transmits:

```text
USART Driver Application

```
----------

### Echo Functionality

The application continuously:

1.  Waits for a string from the serial terminal. 
2.  Stores the received characters in a buffer.    
3.  Detects end of line ('\n' or '\r').  
4.  Transmits the same string back.
    

Example:

PC sends:

```text
Hello STM32
```

STM32 replies:
```text
Hello STM32
```

This demonstrates successful bidirectional USART communication.

----------

## Learning Objectives

This project helps understand:

-   STM32 peripheral programming    
-   USART register configuration    
-   Baud rate generation    
-   Serial communication protocols    
-   Polling-based data transfer    
-   Driver abstraction design    
-   Embedded C programming
    

----------

## Future Improvements

Possible enhancements:

-   Interrupt-based transmission and reception    
-   DMA support    
-   Error handling (Parity, Framing, Overrun)    
-   Ring buffer implementation  
-   Non-blocking APIs
-   Support for variable peripheral clock frequencies    
-   RTOS integration
    
# Demo Video

[Watch STM32 USART Driver Demo](https://youtu.be/PbcRO8e_91U?si=hDwmwrciK2dh-Hk_)
