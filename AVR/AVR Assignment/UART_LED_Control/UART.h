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
void UART_Send_number(unsigned int num);
void UART_Receive_string(char *string, uint8_t buff_size);




#endif /* UART_H_ */