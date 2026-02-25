/*
 * I2c.h
 *
 * Created: 15-01-2026 19:33:24
 *  Author: hp
 */ 

#include <avr/io.h>
#define F_CPU 16000000UL
#ifndef I2C_H_
#define I2C_H_
                                                  // I2c general functions
void I2c_init();
uint8_t I2c_start();
uint8_t I2c_slave_add_write(uint8_t add);
uint8_t I2c_send_byte(uint8_t byte);
uint8_t I2c_slave_add_read(uint8_t add);
uint8_t I2c_read_byte_ack();
uint8_t I2c_read_byte_nack();
void I2c_stop();                         
                                                    // RTC functions
uint8_t RTC_read_byte(uint8_t add);
void RTC_write_byte(uint8_t add,uint8_t data);
uint8_t dec_to_bcd(uint8_t value);
uint8_t bcd_to_dec(uint8_t bcd);

#endif /* I2C_H_ */