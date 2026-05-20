/*
 * BMP280.h
 *
 * Created: 19-05-2026 18:02:41
 *  Author: hp
 */ 


#ifndef BMP280_H_
#define BMP280_H_


uint8_t BMP280_Read_Byte(uint8_t reg_addr);
void BMP280_Read_Multi_Byte(uint8_t reg_addr, uint8_t *buff, uint8_t n);
void BMP280_Write_Byte(uint8_t reg_addr, uint8_t data);

#endif /* BMP280_H_ */