/*
 * SPI.h
 *
 * Created: 19-05-2026 13:26:55
 *  Author: hp
 */ 


#ifndef SPI_H_
#define SPI_H_

// define SPI pins:
#define SS_PIN    PORTB2
#define MOSI_PIN  PORTB3
#define MISO_PIN  PORTB4
#define SCK_PIN   PORTB5
#define SPI_PORT  PORTB

void Init_SPI();
void Init_SPI_Pins();
uint8_t SPI_transfer(uint8_t data);
void Select_slave();
void Deselect_slave();

#endif /* SPI_H_ */