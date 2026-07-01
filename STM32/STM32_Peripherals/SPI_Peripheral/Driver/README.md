

# STM32F401RE SPI Driver 

## Overview

This project implements a **bare-metal SPI driver for STM32F401RE** using direct register access.  
It supports SPI initialization, full-duplex communication, blocking transmit/receive APIs, and error handling.

The driver is designed for **embedded learning + real sensor interfacing**.

----------

## Features

-   SPI master/slave configuration
-   Full-duplex / Half-duplex / Simplex support
-   8-bit / 16-bit data frame support
-   Software Slave Management (SSM)
-   Configurable clock polarity (CPOL) and phase (CPHA)
-   Configurable baud rate prescaler
-   Blocking transmit and receive APIs
-   Flag-based status checking (TXE, RXNE, BSY)
-   Overrun (OVR) flag handling

----------

## Supported SPI Modes


| Mode | Description |
| ----------- | ------------------------- |
| Full Duplex | MOSI + MISO active        |
| Half Duplex | Single-wire bidirectional |
| Simplex RX  | Receive-only mode         |

## Driver Architecture

### Core Layers

-   **SPI Peripheral Control**
    -   Clock enable/disable
    -   Enable/disable SPI peripheral
-   **Configuration Layer**
    -   CPOL / CPHA setup
    -   Data frame format (8/16 bit)
    -   Baud rate selection
    -   Master/slave selection
    -   SSM/SSI configuration
-   **Data Transfer Layer**
    -   Blocking transmit
    -   Blocking receive
-   **Status Layer**
    -   Flag checking (TXE, RXNE, BSY)
-   **Error Handling**
    -   Overrun flag clearing
    
    ## API Functions

### 1. Clock Control
`void SPI_PeriClockControl(SPI_Reg_t *pSPIx, uint8_t EnorDi);`

Enables or disables peripheral clock for SPI instance.

### 2. Peripheral Enable/Disable
```
void SPI_PeripheralControl(SPI_Reg_t *pSPIx, uint8_t EnorDi);
```
Enables or disables SPI peripheral (SPE bit).

### 3. Initialization
```
void SPI_Init(SPI_Handle_t *pSPIHandle);
```

Initializes SPI peripheral based on configuration structure.

### 4. Flag Status Check
```
uint8_t SPI_Get_FlagStatus(SPI_Reg_t *pSPIx, uint8_t Flag);
```
Returns status of SPI flags like TXE, RXNE, BSY.

### 5. Blocking Transmit

```
void SPI_Transmit_Buffer(SPI_Reg_t *pSPIx, uint8_t *pTxBuffer, uint8_t Tx_len);
```

Transmits a data buffer using polling (blocking) method.
In **full-duplex** SPI communication, every transmitted byte also receives a byte internally. The received data must be read to clear `RXNE`.

A **dummy read of SPI_DR** is performed after each byte transfer to prevent overrun issues.
`(void)pSPIx->SPI_DR;`

### 6. Blocking Receive

```
void SPI_Receive_Buffer(SPI_Reg_t *pSPIx, uint8_t *pRxBuffer, uint8_t Rx_len);
```
Receives data using dummy clock generation.

### 7. Clear Overrun Flag
```
void SPI_ClearOVRFlag(SPI_Reg_t* pSPIx);
```
Clears OVR flag by dummy read sequence.

## Key Design Concepts

### 1. Full-Duplex Nature of SPI

-   Every transmit generates a receive
-   **RXNE** must be checked before reading Data Register
-  **TXE** must be checked before writing data to Data Register 
-   Dummy writes (0xFF) are used for clock generation

----------

### 2. Software Slave Management (SSM)

-   SSI must be set in master mode when SSM is enabled
-   Prevents MODF (mode fault)

### 4. Overrun Handling
Overrun occurs when RX data is not read in time.
Fix:

```
temp = SPI_DR;
temp = SPI_SR;
```
## Notes

-   Always wait for `BSY = 0` before deasserting CS
-   Always clear RXNE before new transaction
-   This driver uses **polling (blocking mode)** only
-   Suitable for sensors like BMP280, MPU6050, flash memory, etc.
