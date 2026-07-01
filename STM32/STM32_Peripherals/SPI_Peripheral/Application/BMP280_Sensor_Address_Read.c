/*
 * BMP280_Sensor_Address_Read.c
 *
 *  Created on: May 29, 2026
 *      Author: hp
 */


#include  "stdint.h"
#include  "STM32F401RE.h"
#include  "STM32F401RE_SPI_Driver.h"


int  main(void)
{
      // Enable clock for SPI1
      SPI_PeriClockControl(SPI1, ENABLE);
      
      // Set Alternate functions for GPIO Pins
      SPI_GPIO_Configurations();

      // Set SPI Configurations
      SPI_Handle_t handle;
      handle.pSPIx = SPI1;
      handle.SPI_Config.SPI_Device_Mode = SPI_DEVICE_MODE_MASTER;
      handle.SPI_Config.SPI_DataFrame = SPI_DFF_8BITS;
      handle.SPI_Config.SPI_CLK_Speed = SPI_PRESCALAR_16;
      handle.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
      handle.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
      handle.SPI_Config.SPI_Bus_Config = SPI_BUS_CONFIG_FD;
      
      SPI_Init(&handle);
      
      // Read Sensor ID of sensor BMP280
      // - ID stored at register address 0xD0
      // - In SPI mode, sensor requires MSB=1 for read operations
      // - send register address | MSB=1
      // - send dummy byte to generate clocks and receive sensor ID (0x58)

      // select slave : SS pin (PB6) = LOW
      GPIOB->ODR &=~(1<< 6);
      
      // send register address | MSB=1
      uint8_t  cmd  =  0xD0  |  0x80;  
      SPI_Transmit_Buffer(SPI1, &cmd, 1);
      
      uint8_t sensor_id;
      SPI_Receive_Buffer(SPI1, &sensor_id, 1);
      
      // De-select slave : SS pin = HIGH
      GPIOB->ODR |=(1<< 6);
      
      // Set a Debug LED
      GPIOA->MODER &= ~(0x3<< 0*2);
      GPIOA->MODER |= (1<< 0*2);


      if(sensor_id == 0x58)
      {
           //turn on debug LED
           GPIOA->ODR |= (1<< 0);
       }
       while(1)
       {
       }

}