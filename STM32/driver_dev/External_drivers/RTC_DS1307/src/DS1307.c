/*
 * DS1307.c
 *
 *  Created on: Aug 18, 2026
 *      Author: hp
 */

#include "DS1307.h"


//define a lookup table to print day of week
//- Only for printning/debugging : OPTIONAL
const char *RTC_DayNames[] = { "invalid day",
		               "sunday",
					   "monday",
					   "tuesday",
					   "wednesday",
					   "thursday",
					   "friday",
					   "saturday" };

//helper functions for BCD-decimal conversions

static uint8_t RTC_BCD_To_Decimal(uint8_t bcd)
{
	//converting 0x25 to 25
	uint8_t dec = ((bcd>>4)*10) + (bcd&0x0F);

	return dec;
}
static uint8_t RTC_Decimal_To_BCD(uint8_t dec)
{
	//converting 25 to 0x25
	uint8_t bcd = ((dec/10) << 4) | (dec%10);

	return bcd;
}


//define public APIs

RTC_Status_t DS1307_SetTime(DS1307_t *rtc, RTC_Time_t *time )
{
	//set up a buffer to store time
	uint8_t buffer[3];

	//perform a validation before writing into registers
	if(time->minutes >= 60 || time->seconds >= 60)
	{
		return RTC_ERROR_INVALID_TIME ;
	}
	if((time->hour_mode == RTC_HOURMODE_24 && time->hours >= 24) || (time->hour_mode == RTC_HOURMODE_12 && ((time->hours < 1 )||time->hours > 12)))
	{
		return RTC_ERROR_INVALID_TIME;
	}
	if ((time->hour_mode != RTC_HOURMODE_24) &&
	    (time->hour_mode != RTC_HOURMODE_12))
	{
	    return RTC_ERROR_INVALID_TIME;
	}
	if ((time->hour_mode != RTC_HOURMODE_24) &&(time->am_pm != RTC_AM) &&  (time->am_pm != RTC_PM))
	{
	    return RTC_ERROR_INVALID_TIME;
	}

	//store seconds into buffer (in BCD format)
	//!! Important to keep clock halt bit as LOW: Indicating clock is enabled !!//
	buffer[0] = RTC_Decimal_To_BCD(time->seconds)  & ~DS1307_BIT_SECONDS_CH;

	//store minutes into buffer (in BCD format)
	buffer[1] = RTC_Decimal_To_BCD(time->minutes);

	//store hours into buffer (in BCD format)
	//check if user has set 12 or 24 hour mode
	if(time->hour_mode == RTC_HOURMODE_24)
	{
		//means bit6 in hours register is LOW : bit 5 can be ignored (clear bit6 and bit5)
		buffer[2] = RTC_Decimal_To_BCD(time->hours ) & ~(DS1307_BIT_HOURS_12_24 | DS1307_BIT_HOURS_AMPM);
	}
	else if(time->hour_mode == RTC_HOURMODE_12)
	{

		//mean bit 6 is HIGH : bit 5 indicates AM/PM (0==AM/1==PM)
		if(time->am_pm == RTC_AM)
		{
			// bit5 is 0 : for a valid 12 hour value, bit5 is already 0, only need bit6 as 1
			 buffer[2] = RTC_Decimal_To_BCD(time->hours) | DS1307_BIT_HOURS_12_24;
		}
		if(time->am_pm == RTC_PM)
		{
			// bit5 is 1 : for a valid 12 hour value, bit5 is already 0, need bit6 and bit5 as 1
			 buffer[2] = RTC_Decimal_To_BCD(time->hours)  | DS1307_BIT_HOURS_12_24 | DS1307_BIT_HOURS_AMPM;
		}
	}

	//write the buffer into RTC registers
	I2C_Transmit_Buffer(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS, buffer, 3);

	return RTC_OK;

}

RTC_Status_t DS1307_GetTime(DS1307_t *rtc, RTC_Time_t *time)
{
	//setup a buffer to receive data
	uint8_t buffer[3]={0};

	//receive BCD data from RTC registers
	I2C_Receive_Buffer(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS, buffer, 3);

	//store seconds
	time->seconds = RTC_BCD_To_Decimal(buffer[0]);

	//store minutes
	time->minutes = RTC_BCD_To_Decimal(buffer[1]);

	//store hours
	//check hour mode: check bit6
	if((buffer[2] & DS1307_BIT_HOURS_12_24)  == LOW)// bit6 is LOW: 24hour mode
	{
		time->hour_mode = RTC_HOURMODE_24;//am_pm is ignored
		time->hours     = RTC_BCD_To_Decimal(buffer[2] & 0x3F);

	}
	else// 12hour mode: bit6 and bit5 are control bits,
	{
		time->hour_mode = RTC_HOURMODE_12;

		//check am/pm : check bit5
		if((buffer[2] & DS1307_BIT_HOURS_AMPM) != LOW)//PM
		{
			time->am_pm = RTC_PM;
		}
		else//AM
		{
			time->am_pm = RTC_AM;
		}

		// data is stored at bit4:0
		time->hours = RTC_BCD_To_Decimal(buffer[2] & 0x1F);

	}

	//return an error if clock is halted/ not running
	if((buffer[0] & DS1307_BIT_SECONDS_CH) != LOW)
	{
		return RTC_ERROR_CLOCK_HALTED;
	}
	return RTC_OK;
}

RTC_Status_t DS1307_SetDate(DS1307_t *rtc, RTC_Date_t *date)
{
	//setup a buffer to store the date
	uint8_t buffer[4];

	//perform validation
	if((date->day_of_week < RTC_SUNDAY) ||date->day_of_week > RTC_SATURDAY || date->date > 31  || date->date < 1 || date->month < 1|| date->month > 12 )
	{
		return RTC_ERROR_INVALID_DATE;
	}
	if (date->year < 2000 || date->year > 2099)
	{
	    return RTC_ERROR_INVALID_DATE;// // Driver supports years 2000-2099
	}

	//store day of week
	buffer[0] =RTC_Decimal_To_BCD( date->day_of_week);

	//store date
	buffer[1] = RTC_Decimal_To_BCD(date->date);

	//store month
	buffer[2] = RTC_Decimal_To_BCD(date->month);

	//store year
	buffer[3] = RTC_Decimal_To_BCD((date->year%100));

	//write data into RTC registers
	I2C_Transmit_Buffer(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_DAY, buffer, 4);

	return RTC_OK;
}

RTC_Status_t DS1307_GetDate(DS1307_t *rtc, RTC_Date_t *date)
{
	//setup a buffer to store date
	uint8_t buffer[4] = {0};


	//receive BCD data from RTC registers
	I2C_Receive_Buffer(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_DAY, buffer, 4);

	//store day of week
	date->day_of_week = RTC_BCD_To_Decimal(buffer[0]);

	//store date
	date->date = RTC_BCD_To_Decimal(buffer[1]);

	//store month
	date->month = RTC_BCD_To_Decimal(buffer[2]);

	//store year
	date->year = 2000 + RTC_BCD_To_Decimal(buffer[3]);

	//return an error if clock is halted/ not running
		if((buffer[0] & DS1307_BIT_SECONDS_CH) != LOW)
		{
			return RTC_ERROR_CLOCK_HALTED;
		}

	return RTC_OK;

}

void DS1307_StartClock(DS1307_t *rtc)
{
	//read current seconds
	uint8_t seconds = I2C_Receive_Byte(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS);
	//clear bit7 of seconds
	seconds &=~ DS1307_BIT_SECONDS_CH;
	//write to RTC seconds register
	I2C_Transmit_Byte(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS, seconds);
}
void DS1307_StopClock(DS1307_t *rtc)
{
	//read current seconds
	uint8_t seconds = I2C_Receive_Byte(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS);
	//set bit7 of seconds register
	seconds |= DS1307_BIT_SECONDS_CH;
	//write to RTC seconds register
	I2C_Transmit_Byte(rtc->I2C_Instance, DS1307_ADDRESS, DS1307_REG_SECONDS, seconds );
}














