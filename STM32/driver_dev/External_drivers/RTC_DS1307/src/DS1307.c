/*
 * DS1307.c
 *
 *  Created on: Aug 18, 2026
 *      Author: hp
 */



//helper functions for BCD-decimal conversions

static uint8_t RTC_BCD_To_Decimal(uint8_t bcd);
static uint8_t RTC_Decimal_To_BCD(uint8_t dec);
