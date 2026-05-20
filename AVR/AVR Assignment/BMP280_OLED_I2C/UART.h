/*
 * UART.h
 *
 * Created: 18-05-2026 12:22:27
 *  Author: hp
 */ 


#ifndef UART_H_
#define UART_H_



void Init_UART();
void UART_Send_data(uint8_t data);
uint8_t UART_Receive_data();
void UART_Send_string(char *string);
void UART_Send_number(uint16_t num);
void UART_Receive_string(char *string, uint8_t buff_size);
void UART_Send_HEXnumber(unsigned int num);
void UART_Send_signed_number(int16_t num);
void UART_Send_32bitnumber(uint32_t num);
void UART_Send_32bitsigned(int32_t num);
#endif /* UART_H_ */