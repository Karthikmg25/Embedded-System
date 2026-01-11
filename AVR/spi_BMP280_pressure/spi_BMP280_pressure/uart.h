/*
 * uart.h
 *
 * Created: 27-12-2025 22:16:56
 *  Author: hp
 */ 
#include <avr/io.h>
#include <stdlib.h>

#define F_CPU 16000000UL

#ifndef UART_H_
#define UART_H_
void uart_init();
void TX_char(char c);
void TX_string(char *str);
void TX_num(uint16_t num);
void TX_hex_num(uint8_t hex);
void TX_32bit_num(uint32_t num);
void TX_float(float value);// float sending
void TX_Float_send(float);
char RX_char();
void RX_string(char *str, uint8_t);
uint16_t RX_num(char *str);



#endif /* UART_H_ */