# STM32F401RE SPI Driver 
## Overview 
The driver supports:

* Master / Slave mode 
* Full Duplex, Half Duplex, Simplex RX

* 8-bit and 16-bit data frames

* Software Slave Management (SSM)

* Blocking transmit and receive

* Overrun flag handling 

All configurations are done through `SPI_Handle_t`.

## Driver Architecture  
The driver follows a **Handle-based configuration model**:  

```
typedef struct
{
    SPI_Reg_t *pSPIx;
    SPI_Config_t SPI_Config;
} SPI_Handle_t;
```

Configuration is applied once using `SPI_Init()`, and SPI is enabled separately.

## API Reference

1. `void SPI_PeriClockControl(SPI_Reg_t *pSPIx, uint8_t EnorDi)` 

     Enables or disables clock for SPI1–SPI4.  

2. `void SPI_PeripheralControl(SPI_Reg_t *pSPIx, uint8_t EnorDi)`   

     Enables or disables the SPI peripheral (SPE bit in CR1).   
     Must be called after configuration. 

3. `void SPI_Init()`  

     Initializes SPI based on values inside SPI_Handle_t.  
     Configures:

    * Device Mode (Master / Slave)  
    * CPOL and CPHA  
    * Bus configuration (FD / HD / Simplex RX)  
    * Data Frame Format (8-bit / 16-bit)  
    * Software Slave Management  
    * Baud Rate Prescaler  

      Clock is enabled inside this function. SPI is **not enabled automatically**.  

4. `uint8_t SPI_Get_FlagStatus(SPI_Reg_t *pSPIx, uint8_t Flag)`   

     Returns status of a given SPI flag.   
     Used internally or for polling.  

5. `void SPI_Transmit(SPI_Reg_t *pSPIx, uint8_t *pTxBuffer, uint8_t Tx_len)`  

    Behavior:

    * Waits until TXE is set
    * Writes data based on DFF (8/16-bit) 
    * Waits until BSY flag clears 
    * Blocking implementation 
    * Does not use interrupts 
    
      Note:  
      RXNE clearing inside transmit is commented. Overrun must be handled if needed.

6. `void SPI_Receive(SPI_Reg_t *pSPIx, uint8_t *pRxBuffer, uint8_t Rx_len)`  

    Behavior:

    * Waits for RXNE
    * Reads DR based on DFF (8/16-bit) 
    * Waits until BSY flag clears 
    * Blocking implementation 
    
      Note:  
      Dummy write logic is left to user (commented section).

7. `void SPI_ClearOVRFlag(SPI_Reg_t* pSPIx)`   

     Clears Overrun (OVR) flag by:  
       1. Reading DR  
       2. Reading SR  

     Required if RXNE is not cleared during transmit. 

## Design Characteristics


* Fully register-level
* Blocking implementation

* No interrupt or DMA support

* Suitable for simple polling-based SPI communication

## Usage Flow (Master Mode Example)

1. Configure GPIO Pins for SPI functions 
2. Configure `SPI_Handle_t`  
3. Call `SPI_Init()`
4. Call `SPI_PeripheralControl(..., ENABLE)`  
5. Call `SPI_Transmit()` or `SPI_Receive()`  
6. Disable SPI if required  

## Tested


* Loopback test (MOSI connected to MISO)
* 8-bit frame transmission
* Blocking transmit and receive



