/*
 * RTC_DriverInterface.c
 *
 *  Created on: Aug 20, 2026
 *      Author: hp
 */



#include <stdio.h>
#include "DS1307.h"

//Application is using I2C1 instance for communication
static void I2C1_Configurations();
static void GPIO_Configurations_SCL_SDA();
static void force_delay();

int App3_main(void)
{
	GPIO_Configurations_SCL_SDA();

	I2C1_Configurations();

	//declare an RTC device instance
	/*****************************************************************/

	DS1307_t rtc;
	rtc.I2C_Instance = I2C1;
	rtc.device_address = DS1307_ADDRESS;

	// time and date configuration
	/*****************************************************************/
	RTC_Time_t time;
	time.seconds   = 52;
	time.minutes   = 59;
	time.hours     = 11;
	time.hour_mode = RTC_HOURMODE_12;
	time.am_pm     = RTC_PM;

	RTC_Date_t date;
	date.day_of_week   = RTC_THURSDAY;
	date.date          = 31;
	date.month         = 8;
	date.year          = 2026;

	//set date and time:

	RTC_Status_t time_status, date_status;

	time_status = DS1307_SetTime(&rtc, &time);
	date_status = DS1307_SetDate(&rtc, &date);

	if(time_status != RTC_OK || date_status != RTC_OK)
	{
		printf("RTC Time/Date Configuration error");
	}

	while(1)
	{
		//Read RTC data continuously inside the loop
		/******************************************************************/

		time_status = DS1307_GetTime(&rtc, &time);
		date_status = DS1307_GetDate(&rtc, &date);

		if(time_status != RTC_OK || date_status != RTC_OK)
		{
			printf("\n\nERROR\n\n");
		}

		//Print Time and Date in the console
		/*****************************************************************/

		printf("\n   RTC Time and date \n");
		printf("_________________________\n\n");


		printf("Time      - %02d : %02d : %02d ", (int)time.hours, (int)time.minutes, (int)time.seconds);

		if(time.hour_mode == RTC_HOURMODE_24)
		{
			printf("%s", "(24 hour)\n");
		}
		else
			if(time.hour_mode == RTC_HOURMODE_12)
		{
			if(time.am_pm == RTC_AM)
			{
				printf(" %s", "AM\n");
			}
			else//PM
			{
				printf(" %s", "PM\n");
			}
		}

		printf("Date      - %02d : %02d : %d\n\n", (int)date.date,  (int)date.month,  (int)date.year);

		printf("Day of the week: %s\n", RTC_DayNames[date.day_of_week]);

		force_delay();

	}
}









static void force_delay()
{
	for(int i=0; i<200000;i++);
}
void GPIO_Configurations_SCL_SDA()
{

	// Set GPIO Configurations
	// - PB8 and PB9 are used as SCl and SDA of I2C1 instance
	// - Use alternate function AF4
	// - SCL and SDA are open drain with pull up
	GPIOB_CLK_EN();
	GPIOB->MODER  &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->MODER  |= (0x2<< 8*2)|(0x2<< 9*2);
	GPIOB->AFR[1] &=~((0xF<<0*4)|(0xF<< 1*4));
	GPIOB->AFR[1] |= ((0x4<<0*4)|(0x4<< 1*4));
	GPIOB->OTYPER |= (1<< 8)|(1<< 9);
	GPIOB->PUPDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->PUPDR |= ((0x1<< 8*2)|(0x1<< 9*2));
	GPIOB->OSPEEDR &=~((0x3<< 8*2)|(0x3<< 9*2));
	GPIOB->OSPEEDR |=((0x3<< 8*2)|(0x3<< 9*2));

}
static void I2C1_Configurations()
{
	I2C_Handle_t i2c1;

	i2c1.pI2Cx = I2C1;
	i2c1.I2C_Config.I2C_SCL_Speed = I2C_SCL_SPEED_SM;

	I2C_Init(&i2c1);

	I2C_PeripheralControl(I2C1, ENABLE);
}



