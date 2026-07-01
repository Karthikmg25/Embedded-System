


# RTC Interface Using STM32 I2C Driver

This application demonstrates how to use the polling-based STM32F401 I2C driver to interface with an RTC (Real-Time Clock) device.

The application:

-   Configures I2C1 as the controller (master)
    
-   Initializes the RTC with a predefined time
    
-   Reads the current time from the RTC periodically
    
-   Converts RTC BCD data into decimal format
    
-   Displays the current time through USART
    

## Hardware Used
|Component| Description |
|---------------|-------------|
|STM32F401RE    | Controller |
| DS1307        | I2C Real-Time Clock |
| USART Terminal| Debug Output |

## RTC Information

|Parameter| Value |
|-----------------|-------------|
|Slave Address    | 0x68|
| Seconds Register| 0x00 |
| Minutes Register| 0x01|
|Hours Register   | 0x02|

### Register Notes

#### Seconds Register (0x00)

|Bit| Description|
|-----------------|-------------|
|7   |Clock Halt (CH) Bit|
|6:0|Seconds Value |
For normal RTC operation:

```c
seconds &= 0x7F;

```

#### Hours Register (0x02)
|Bit| Description|
|-----------------|-------------|
|6  |12/24 Hour Mode|
|5|AM/PM Indicator (12-hour mode) |
|5:0|Hour Value (24-hour mode)|

For 24-hour operation:

```c
hours &= 0x3F;

```

## Application Flow

### 1. Configure GPIO

PB8 and PB9 are configured for I2C1 operation.
|Pin      | Function   | Alternate Function|
|--------|-----------------------|---------|
|PB8     |   I2C1_SCL    | AF4
|PB9     |   I2C1_SDA    | AF4

### 2. Initialize I2C Peripheral

```c
I2C_Handle_t handle;

handle.I2Cx = I2C1;
handle.I2C_Config.I2C_Speed_mode = Standard_Mode;
handle.I2C_Config.I2C_SCL_Speed = 100000;

I2C_Init(&handle);

```

### 3. Prepare Time Data

The RTC expects data in Binary Coded Decimal (BCD) format.

Example:
| Decimal | BCD |
|----------|----------|
| 3 | 0x03 |
| 27 | 0x27 |
| 22| 0x22 |


Time values are converted before transmission.

```c
buffer[0] = ((((seconds/10) << 4) | (seconds%10)) & 0x7F);
buffer[1] = (((minutes/10) << 4) | (minutes%10));
buffer[2] = ((((hours/10) << 4) | (hours%10)) & 0x3F);

```

### 4. Initialize RTC Time

Transmit seconds, minutes and hours starting from register address 0x00.

```c
I2C_Transmit_Buffer(
    I2C1,
    0x00,
    buffer,
    3,
    RTC_ADDR
);

```

### 5. Read Current Time

The application periodically reads:

-   Seconds
    
-   Minutes
    
-   Hours
    

from the RTC.

```c
I2C_Receive_Buffer(
    I2C1,
    0x00,
    buffer,
    3,
    RTC_ADDR
);

```

### 6. Convert BCD to Decimal

```c
seconds = ((buffer[0] >> 4) * 10) + (buffer[0] & 0x0F);

minutes = ((buffer[1] >> 4) * 10) + (buffer[1] & 0x0F);

hours =   ((buffer[2] >> 4) * 10) + (buffer[2] & 0x0F);

```

### 7. Send Time to USART Terminal

```c
USART_Send_String("\n\nCurrent Time: ");

USART_Send_Number(hours);
USART_Send_String(": ");

USART_Send_Number(minutes);
USART_Send_String(": ");

USART_Send_Number(seconds);

```

## Example Output

```text
Current Time: 23: 10: 03

Current Time: 23: 10: 04

Current Time: 23: 10: 05

Current Time: 23: 10: 06

```

## Demonstrated Driver APIs


| API         | Purpose |
|------------|----------|
| I2C_Init()|Initialize I2C peripheral |
| I2C_Transmit_Buffer() |Write data to RTC registers|
|I2C_Receive_Buffer()   |Read data from RTC registers |




## Learning Objectives

This application demonstrates:

-   Practical I2C communication
    
-   RTC interfacing
    
-   Register-based peripheral access
    
-   BCD encoding and decoding
    
-   Multi-byte I2C transactions
    
-   STM32 bare-metal firmware development
    
-   USART-based debugging

# Demo Video

[Watch STM32 I2C Driver Demo](https://youtube.com/shorts/43GTIjzq2JA?si=0MxBK_fLVHNGp8vV)