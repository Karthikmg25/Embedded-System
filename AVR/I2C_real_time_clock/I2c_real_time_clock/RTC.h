/*
 * RTC.h
 *
 * Created: 22-01-2026 17:40:52
 *  Author: hp
 */ 


#ifndef RTC_H_
#define RTC_H_
void RTC_write_time();
void RTC_read_time(uint8_t time[3]);
uint8_t dec_to_bcd(uint8_t value);
uint8_t bcd_to_dec(uint8_t bcd);
void RTC_write_byte(uint8_t add, uint8_t data);
uint8_t RTC_read_byte(uint8_t add);



#endif /* RTC_H_ */