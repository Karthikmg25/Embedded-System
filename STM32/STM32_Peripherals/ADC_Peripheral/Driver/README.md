


# STM32F401RE ADC Driver

## Overview

This project implements a lightweight bare-metal ADC driver for the STM32F401RE microcontroller.

The driver is designed with a focus on:

-   Simplicity
-   Reliability
-   Interrupt-driven operation
-   Clean driver architecture
-   Application-layer flexibility
- 
# Hardware

-   Microcontroller: STM32F401RE
-   Architecture: ARM Cortex-M4
-   Board: NUCLEO-F401RE
-   Flash: 512KB
-   SRAM: 96KB

# Driver Features

## Supported Features

-   Interrupt-driven ADC conversion
-   Single-shot conversion
-   Runtime channel selection
-   Callback function support
-   Conversion completion synchronization flag
-   Stores latest ADC result inside handle structure
-   Software-controlled continuous conversion
-   Software-controlled multi-channel sequencing

## Unsupported Hardware Features

The following STM32 ADC hardware features are intentionally not implemented in this driver:

-   SCAN mode
-   Continuous mode
-   Discontinuous mode
-   Injected channels
-   Analog watchdog
-   DMA support

These features were excluded to keep the driver:

-   easier to understand
-   easier to debug
-   suitable for learning low-level peripheral development

# Driver Architecture

The driver is separated into multiple logical layers.

## 1. Driver Layer

Responsible for:

-   ADC initialization
-   Register configuration
-   Starting conversions
-   Interrupt handling


```
## 2. ISR/Event Layer

Responsible for:

-   Handling ADC interrupts
-   Reading ADC conversion results
-   Updating synchronization flags
-   Invoking callback functions
## 3. Application Layer

Responsible for:

-   Application-specific logic
-   Deciding what to do with ADC results
-   Peripheral Initialisation 
-   Starting next conversions
-   Implementing software sequencing

Example:
```
void Toggle_LED_BasedOn_ADC_Result(uint16_t result)
{
    if(result > 4000)
    {
        GPIOA->ODR |= (1<<1);
    }
    else
    {
        GPIOA->ODR &= ~(1<<1);
    }
}
```
# Driver Design Philosophy

Instead of relying on hardware continuous conversion or SCAN mode, this driver uses:

-   interrupt completion events
-   software-controlled conversion sequencing

This approach provides:

-   deterministic execution flow
-   easier debugging
-   improved educational value
-   simpler state management

# ADC Handle Structure

The handle structure stores:

-   ADC configuration parameters
-   Latest ADC conversion result
-   Conversion completion status
-   Callback function pointer

Example:
```
typedef struct
{
    ADC_Config_t ADC_config;

    volatile uint16_t ADC_Result;

    volatile uint8_t ADC_Conversion_Complete;

    void (*function_ptr)(uint16_t result);

}ADC_Handle_t;
```
# Application Conversion Flow

## Step 1 — Initialize Driver

```
ADC_Init(&handle);
```

Configures:

-   ADC resolution
-   ADC clock prescaler
-   interrupt generation
-   NVIC configuration
-   ADC peripheral enable
- 
## Step 2 — Register Callback

```
ADC_RegisterCallback(&handle, Toggle_LED_BasedOn_ADC_Result);
```

Registers a user-defined callback function with the ADC driver.

After ADC conversion completes, the ISR automatically invokes this function and passes the latest ADC result as an argument.

This mechanism allows the application layer to define custom behavior after conversion without hardcoding application logic inside the driver.


## Step 3 — Configure GPIO Pin as ADC 
```
ChannelcGPIO_Configuration_Channel_0();
```

Configures the GPIO pin connected to the ADC channel in analog mode.

For STM32F401RE:

-   ADC Channel 0 maps to PA0

The pin must be configured in analog mode before starting ADC conversion to ensure proper analog signal sampling.


## Step 4 — Start Conversion

```
ADC_StartConversion(&handle, channel_0);
```

The driver:

-   selects ADC channel
-   configures sample time
-   clears conversion status
-   starts ADC conversion

## Step 5 — Interrupt Handling

When conversion completes:

-   ADC interrupt is triggered
-   ADC result is read
-   EOC flag is cleared automatically
-   Conversion complete flag is updated
-   Callback function is invoked

## Software Continuous Conversion

Continuous conversion is implemented in software using the conversion completion flag.

Example:

```
while(1)
{
    if(handle.ADC_Conversion_Complete)
    {
        ADC_StartConversion(&handle, channel_0);
    }
}

```
Next conversion is performed when previous one is complete

This approach avoids hardware CONT mode while still enabling repeated conversions.



# Files

## Driver Files

```
STM32F401RE_ADC_Driver.c
STM32F401RE_ADC_Driver.h
```
## Application File

```
main.c
```

# Educational Concepts Covered

This project demonstrates:

-   Bare-metal peripheral programming
-   STM32 ADC register configuration
-   Interrupt handling
-   NVIC programming
-   Driver abstraction
-   Callback mechanisms
-   ISR synchronization
-   Software sequencing
-   Embedded driver architecture

# Future Improvements

Possible future extensions:

-   Multi-instance ADC support
-   Hardware SCAN mode
-   Continuous conversion mode
-   DMA support
-   Circular buffer support
-   ADC watchdog support
-   Driver state machine

# API Functions
```
void  ADC_Init(ADC_Handle_t* pADC_handle);

void  ADC_StartConversion(ADC_Handle_t* pADC_Handle, Channel_t channel);

void  ADC_RegisterCallback(ADC_Handle_t* pADC_Handle, void(*fn_ptr)(uint16_t result));

```
# Usage Example

## LED Control Using ADC Value
```
#include  "STM32F401RE.h"
#include  "STM32F401RE_ADC_Driver.h"

int  main(void)
{
     // Enable ADC clock
     ADC1_CLK_EN();
     
     // Declare an ADC handle & Set configurations
     ADC_Handle_t handle;
     handle.ADC_config.ADC_Prescaler = prescaler_8;
     handle.ADC_config.ADC_Resolution = resolution_12bit;
     handle.ADC_config.ADC_Sample_time = sampletime_144cycle;
     
     ADC_Init(&handle);
     
     // Register call back function
     ADC_RegisterCallback( &handle, Toggle_LED_BasedOn_ADC_Result );

      // GPIO Configuration:
      GPIO_Configuration_Channel_0();

      // Set PA1 as Output for LED
      GPIO_Configuration_LED();

      // Start first conversion
      ADC_StartConversion(&handle, channel_0);

      while(1)
      {
          // Start next conversion if previous one is over
          if(handle.ADC_Conversion_Complete)
          {
              ADC_StartConversion(&handle, channel_0);
           }

       }
}
```

# Demo Video

[Watch STM32 ADC Driver Demo](https://youtube.com/shorts/goXKt1YcoUA?si=ViGGAXchNG9SiTI3)