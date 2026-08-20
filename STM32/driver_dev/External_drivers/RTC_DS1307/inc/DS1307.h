/*
 * DS1307.h
 *
 *  Created on: Aug 18, 2026
 *      Author: hp
 */

#ifndef RTC_DS1307_INC_DS1307_H_
#define RTC_DS1307_INC_DS1307_H_

#include "stm32f401re_i2c_driver.h"

//define device address
#define DS1307_ADDRESS 0x68


//define enums for day of the week
typedef enum
{
	RTC_SUNDAY = 1,
	RTC_MONDAY = 2,
	RTC_TUESDAY= 3,
	RTC_WEDNESDAY=4,
	RTC_THURSDAY=5,
	RTC_FRIDAY=6,
	RTC_SATURDAY=7

}RTC_DayOfWeek_t;

//declare the lookup table to print day of week
//- Only for printning/debugging : OPTIONAL

extern const char *RTC_DayNames[];

//define structures and enums for storing time and date
/*********************************************************************/

//define enum for hour-mode
typedef enum
{
	RTC_HOURMODE_24 = 0,
	RTC_HOURMODE_12 = 1

}RTC_HourMode_t;

//define enum for am/pm
typedef enum
{
	RTC_AM = 0,
	RTC_PM = 1

}RTC_AMPM_t;

typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	RTC_HourMode_t hour_mode;
	RTC_AMPM_t am_pm;

}RTC_Time_t;
typedef struct
{
	RTC_DayOfWeek_t day_of_week;
	uint8_t date;
	uint8_t month;
	uint16_t year;
}RTC_Date_t;

//define an enum for returning status
typedef enum
{
	RTC_OK,
	RTC_ERROR_INVALID_TIME,
	RTC_ERROR_INVALID_DATE,
	RTC_ERROR_CLOCK_HALTED
}RTC_Status_t;


//define structure for DS1307 RTC device
/***********************************************************************/
typedef struct
{
	I2C_Reg_t *I2C_Instance;
	uint8_t device_address;
	// add square wave frequecy, RAM details later

}DS1307_t;


//declare public APIs
/***********************************************************************/

RTC_Status_t DS1307_SetTime(DS1307_t *rtc, RTC_Time_t *time);
RTC_Status_t DS1307_GetTime(DS1307_t *rtc, RTC_Time_t *time);
RTC_Status_t DS1307_SetDate(DS1307_t *rtc, RTC_Date_t *date);
RTC_Status_t DS1307_GetDate(DS1307_t *rtc, RTC_Date_t *date);

void DS1307_StopClock(DS1307_t *rtc);
void DS1307_StartClock(DS1307_t *rtc);

// define DS1307 registers
/**********************************************************************/

#define DS1307_REG_SECONDS      0x00
#define DS1307_REG_MINUTES      0x01
#define DS1307_REG_HOURS        0x02
#define DS1307_REG_DAY          0x03
#define DS1307_REG_DATE         0x04
#define DS1307_REG_MONTH        0x05
#define DS1307_REG_YEAR         0x06
#define DS1307_REG_CONTROL      0x07

//later for including RTC RAM usage:
#define DS1307_REG_RAM_START   0x08
#define DS1307_REG_RAM_END     0x3F

//define important bits in registers
#define DS1307_BIT_SECONDS_CH        (1U<< 7)  // clock-halt bit

#define DS1307_BIT_HOURS_12_24       (1U << 6) // decides 12/24 hour mode
#define DS1307_BIT_HOURS_AMPM        (1U << 5) // decides AM/PM in 12hour mode


//later for implementing square wave feature
#define DS1307_BIT_CTRL_OUT          (1U << 7)
#define DS1307_BIT_CTRL_SQWE         (1U << 4)
#define DS1307_BIT_CTRL_RS1          (1U << 1)
#define DS1307_BIT_CTRL_RS0          (1U << 0)


#endif /* RTC_DS1307_INC_DS1307_H_ */
