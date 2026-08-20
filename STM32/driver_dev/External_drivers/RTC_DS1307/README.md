


# DS1307 I2C Driver for STM32F401RE

## Overview

This driver provides a lightweight, hardware-abstracted interface for setting and reading time and date on the Maxim DS1307 RTC over I2C. It is designed for STM32F401RE bare-metal projects using I2C peripheral. The driver handles BCD ↔ decimal conversion, 12/24-hour mode selection, AM/PM flags, day-of-week encoding, input validation, and clock start/stop (halt-bit) control.

## Features

**Time set/get**  – set and read hours, minutes, seconds with automatic BCD conversion.  
**Date set/get**  – set and read day of week, date, month, and year (2000–2099).  
**12/24-hour support**  – selects hour mode and AM/PM flag directly in the hours register.  
**Input validation**  – rejects out-of-range time/date values before writing registers.  
**Clock halt control**  –  `DS1307_StartClock()`/`DS1307_StopClock()`  clear/set the clock-halt bit.  
**Clock-halted detection**  – getters return an error if the clock is not running.  
**Interface agnostic**  – works with any I2C driver instance by passing a pointer to the I2C registers.  
**Simple APIs**  – six public functions covering set/get time, set/get date, and clock control. \

Language: C (C99)  
Target: STM32F401RE  
Dependencies: Custom I2C driver (stm32f401re_i2c_driver.h)  
Sensor: Maxim DS1307

## Setup

-   Add  `DS1307.c`  and  `DS1307.h`  to your project.
-   Ensure you have implementations for the following functions from your I2C driver:

c

`I2C_Transmit_Buffer(I2C_Reg_t*, uint8_t dev_addr, uint8_t reg, uint8_t*, uint8_t len)  
I2C_Receive_Buffer(I2C_Reg_t*, uint8_t dev_addr, uint8_t reg, uint8_t*, uint8_t len)  I2C_Transmit_Byte(I2C_Reg_t*, uint8_t dev_addr, uint8_t reg, uint8_t data)  I2C_Receive_Byte(I2C_Reg_t*, uint8_t dev_addr, uint8_t reg)`

-   Initialize your I2C peripheral before calling the driver.
-   Include  `DS1307.h`  in your application code.

Note: The driver does not initialize I2C. You must configure the peripheral separately per your project’s requirements.

## Usage

Minimal Flow:

-   Create a  `DS1307_t`  structure.
-   Assign the I2C instance to  `rtc.I2C_Instance`.
-   Optionally set  `rtc.device_address`  (defaults to  `DS1307_ADDRESS`).
-   Set time/date, then read them back.

## code:

c
```
#include "DS1307.h"
int main()
{  
    // First configure I2C1  
    DS1307_t rtc;  
    rtc.I2C_Instance  =  I2C1; 
    rtc.device_address  =  0x68;// DS1307 default address  
    
    /* Ensure the clock is running before setting values */ 
    DS1307_StartClock(&rtc);  
 
    /* Prepare time value (24-hour mode) */  
    RTC_Time_t time;  
    time.hour_mode  =  RTC_HOURMODE_24;
    time.hours  =  14;  
    time.minutes  =  30;  
    time.seconds  =  0;
      
    if  (DS1307_SetTime(&rtc,  &time)  !=  RTC_OK)
    {   // handle invalid time (e.g. minutes >= 60)  }
      
    /* Prepare date value */ 
     RTC_Date_t date; 
     date.day_of_week  =  RTC_WEDNESDAY;  
     date.date  =  18;  
     date.month  =  8;  
     date.year  =  2026;
       
     DS1307_SetDate(&rtc,  &date);  
     
     while(1)
     {  
       RTC_Time_t now; 
       if  (DS1307_GetTime(&rtc,  &now)  ==  RTC_OK)
       {   //read now.seconds, now.minutes, now.hours (24h)
       }  
       RTC_Date_t today;  
       if  (DS1307_GetDate(&rtc,  &today)  ==  RTC_OK)
       {   // today.date, today.month, today.year   
         // print with RTC_DayNames[today.day_of_week]
       }
        
      }  
    }`
  ```

### 12-hour mode example

Set  `time.hour_mode = RTC_HOURMODE_12`  and provide  `time.am_pm = RTC_AM`  or  `RTC_PM`.
 The driver encodes the 12/24 and AM/PM control bits into the hours register:

c
```
RTC_Time_t time;    
   time.hour_mode  =  RTC_HOURMODE_12;  
   time.am_pm    =  RTC_PM;// afternoon  
   time.hours    =  3;  // 3 PM  
   time.minutes  =  45;  
   time.seconds  =  10;  
DS1307_SetTime(&rtc,  &time);
```

When reading back in 12-hour mode,  `time.hour_mode`  and  `time.am_pm`  will be populated and  `time.hours`  holds the 1–12 hour value.

## API Documentation

-   `RTC_Status_t DS1307_SetTime(DS1307_t *rtc, RTC_Time_t *time)` 
    Validates and writes seconds, minutes, and hours (with 12/24 and AM/PM bits) to registers 0x00–0x02. The clock-halt bit is kept low so the clock remains enabled.
    
-   `RTC_Status_t DS1307_GetTime(DS1307_t *rtc, RTC_Time_t *time)` 
 Reads BCD time from registers 0x00–0x02, decodes hour mode and AM/PM, and returns  `RTC_ERROR_CLOCK_HALTED`  if the clock is stopped.
    
-   `RTC_Status_t DS1307_SetDate(DS1307_t *rtc, RTC_Date_t *date)`  
Validates day of week (1–7), date (1–31), month (1–12), and year (2000–2099), then writes day/date/month/year to registers 0x03–0x06.
    
-   `RTC_Status_t DS1307_GetDate(DS1307_t *rtc, RTC_Date_t *date)`  
Reads BCD date registers 0x03–0x06, strips the century offset (adds 2000 to the year), and returns  `RTC_ERROR_CLOCK_HALTED`  if the clock is stopped.
    
-   `void DS1307_StartClock(DS1307_t *rtc)` 
 Clears bit 7 of the seconds register to resume the clock.
    
-   `void DS1307_StopClock(DS1307_t *rtc)` 
Sets bit 7 of the seconds register to halt the clock.
    

## Error handling

All APIs that write to the device return a status enum:
## Error handling

All APIs that write to or read from the device return a status enum.

| Return value | Meaning                                        |
| ------------ | ---------------------------------------------- |
| `RTC_OK`                    | Operation succeeded                            |
| `RTC_ERROR_INVALID_TIME`    | Seconds ≥ 60, minutes ≥ 60, hours out of range for the selected hour mode, unknown hour mode, or invalid AM/PM value |
| `RTC_ERROR_INVALID_DATE`    | Day-of-week outside 1–7, date outside 1–31, month outside 1–12, or year outside 2000–2099 |
| `RTC_ERROR_CLOCK_HALTED`    | Getter called while the clock-halt bit is set (clock stopped) |
Getters return  `RTC_OK`  while still populating the time/date; check the return value to detect a halted clock. SetTime always leaves interrupts the clock enabled, so repeated writes keep the clock running.

## Testing

No formal test suite is included. Basic verification steps:

-   After  `DS1307_SetTime()`, read back with  `DS1307_GetTime()`  and confirm the rounded values match.
-   Verify 12-hour mode toggles  `am_pm`  correctly (PM stores bit 5 HIGH, AM keeps it LOW).
-   Set an impossible value (e.g. minutes = 60) and confirm  `RTC_ERROR_INVALID_TIME`  is returned.
-   Call  `DS1307_StopClock()`, read time, and confirm  `RTC_ERROR_CLOCK_HALTED`; then  `DS1307_StartClock()`  and confirm reads succeed.

## Deployment

This driver is intended for bare-metal embedded use. It compiles with standard C89/C99 and has no OS dependencies. The BCD lookup day-name table (`RTC_DayNames`) is optional and used only for printing/debugging.