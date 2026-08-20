/*
 * DS1307.h
 *
 *  Created on: Aug 18, 2026
 *      Author: hp
 */

#ifndef RTC_DS1307_INC_DS1307_H_
#define RTC_DS1307_INC_DS1307_H_

#include "stm32f401re_gpio_driver.h"
#include "stm32f401re_i2c_driver.h"

//define device address
#define RTC_ADDRESS 0x68


//define enums for day of the week
typedef enum
{
	RTC_sunday = 1,
	RTC_monday = 2,
	RTC_tuesday= 3,
	RTC_wednesday=4,
	RTC_thursaday=5,
	RTC_friday=6,
	RTC_saturday=7

}RTC_DayOfWeek_t;

const char *day[] = { "invalid day",
		               "sunday",
					   "monday",
					   "tuesday",
					   "wednesday",
					   "thursday",
					   "friday",
					   "saturday" };
//define enum for hour-mode
typedef enum
{
	RTC_HourMode24 = 0,
	RTC_HourMode12 = 1
}RTC_HourMode_t;
//define enum for am/pm
typedef enum
{
	RTC_am = 0,
	RTC_pm = 1

}RTC_am_pm_t;
//define structures for storing time and date
typedef struct
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	RTC_HourMode_t hr_mode;
	RTC_am_pm_t am_pm;

}RTC_time_t;
typedef struct
{
	RTC_DayOfWeek_t day_of_week;
	uint8_t date;
	uint8_t month;
	uint16_t year;
}RTC_date_t;


//define structure for DS1307 RTC device
typedef struct
{
	I2C_Reg_t I2C_Instance;
	uint8_t device_address;

}DS1307_t;



//define public APIs

void DS1307_SetTime(DS1307_t *rtc, RTC_time_t *time);
void DS1307_GetTime(DS1307_t *rtc, RTC_time_t *time);
void DS1307_SetDate(DS1307_t *rtc, RTC_date_t *date);
void DS1307_GetDate(DS1307_t *rtc, RTC_date_t *date);


#endif /* RTC_DS1307_INC_DS1307_H_ */
