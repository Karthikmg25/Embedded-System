/*
 * SSD1306.h
 *
 *  Created on: Aug 24, 2026
 *      Author: hp
 */

#ifndef SSD1306_DISPLAY_INC_SSD1306_H_
#define SSD1306_DISPLAY_INC_SSD1306_H_


/**
 * @file SSD1306.h
 * @brief Low-level bare-metal driver for the SSD1306 128x64 monochrome
 *        OLED controller, communicating over I2C.
 *
 * Responsible for: controller initialization, orientation/contrast
 * configuration, display on/off/invert control, and pushing the local
 * framebuffer (SSD1306_t.frame_buffer) to the display's RAM. Does not
 * contain any drawing primitives (pixels/lines/shapes/bitmaps/text) -
 * see SSD1306_Graphics.h for those.
 *
 * Depends on a project-supplied I2C driver (stm32f401re_i2c_driver.h,
 * providing I2C_Reg_t / I2C_Transmit_Byte() / I2C_Transmit_Buffer()).
 */

#include "stm32f401re_i2c_driver.h"
#include <stdio.h>

// * Display hardware specifications *

#define SSD1306_WIDTH        128
#define SSD1306_HEIGHT        64

#define SSD1306_PAGES        (SSD1306_HEIGHT / 8)
#define SSD1306_FRAME_SIZE   (SSD1306_WIDTH * SSD1306_HEIGHT / 8)

/**
 * @brief Display rotation/mirroring options, set once in SSD1306_t.orientation
 *        before calling SSD1306_Init().
 */
typedef enum
{
	OLED_ORIENTATION_NORMAL,
	OLED_ORIENTATION_HORIZONTAL_FLIP,
	OLED_ORIENTATION_VERTICAL_FLIP,
	OLED_ORIENTATION_ROTATE180,

}OLED_Orientation_t;
/**
 * @brief Device/instance handle for one SSD1306 OLED display.
 *
 *        One SSD1306_t exists per physical display. All SSD1306_GFX_
 *        APIs take a pointer to this structure. I2C_interface, I2C_Address,
 *        orientation and contrast must be set by the caller before
 *        SSD1306_Init() is called; frame_buffer is managed internally by
 *        the driver/graphics layer and should not be written directly.
 */
typedef struct
{
	I2C_Reg_t *I2C_interface;
	OLED_Orientation_t orientation;//orientation
	uint8_t contrast;//contrast (8-bitvalue)
	uint8_t I2C_Address;//i2c address
	uint8_t frame_buffer[SSD1306_FRAME_SIZE];

}SSD1306_t;

//define an enum to return status
typedef enum
{
	OLED_OKAY,
	OLED_ERROR_INVALID_COLUMN,
	OLED_ERROR_INVALID_PAGE,
	OLED_ERROR_INVALID_CHARACTER,
	OLED_ERROR_INVALID_ORIENTATION,
	OLED_ERROR_INVALID_COORDINATE,
	OLED_ERROR_BUFFER_OVERFLOW
}OLED_Status_t;


/**
 * @brief Common status/error codes returned by the SSD1306 and GFX APIs.
 */
#define OLED_ADDRESS 0x3C

typedef uint8_t column;   // 0-127 columns
typedef uint8_t page_number ; // 0-7  pages

//OLED APIs

OLED_Status_t SSD1306_Init(SSD1306_t *oled);

void SSD1306_ClearDisplay(SSD1306_t *oled);

void SSD1306_Update(SSD1306_t *oled);// Updates the display after frame buffer modification

void SSD1306_SetContrast(SSD1306_t *oled, uint8_t contrast);

void SSD1306_DisplayInvert(SSD1306_t *oled, bool invert);

void SSD1306_DisplayOff(SSD1306_t *oled);

void SSD1306_DisplayOn(SSD1306_t *oled);


//define macros

#define SSD1306_DATA         0x40
#define SSD1306_COMMAND      0x00


//define SSD1306 commands
#define SSD1306_CMD_DISPLAY_OFF             0xAE
#define SSD1306_CMD_DISPLAY_ON              0xAF

#define SSD1306_CMD_SET_CONTRAST            0x81
#define SSD1306_CMD_SET_DISPLAY_START_LINE  0x40

#define SSD1306_CMD_SET_SEGMENT_REMAP       0xA0
#define SSD1306_CMD_SET_SEGMENT_REMAP_INV   0xA1
#define SSD1306_CMD_SET_COM_SCAN_NORMAL     0xC0
#define SSD1306_CMD_SET_COM_SCAN_REVERSE    0xC8

#define SSD1306_CMD_SET_DISPLAY_OFFSET      0xD3
#define SSD1306_CMD_SET_DISPLAY_CLOCK       0xD5

#define SSD1306_CMD_SET_PRECHARGE_PERIOD    0xD9
#define SSD1306_CMD_SET_COM_PINS_CONFIG     0xDA

#define SSD1306_CMD_SET_VCOMH               0xDB
#define SSD1306_CMD_SET_MULTIPLEX           0xA8
#define SSD1306_CMD_SET_ADDRESSING_MODE     0x20
#define SSD1306_CMD_SET_CHARGE_PUMP         0x8D
#define SSD1306_CMD_DISPLAY_NORMAL          0xA6
#define SSD1306_CMD_DISPLAY_INVERTED        0xA7
#define SSD1306_CMD_DISPLAY_RAM             0xA4
#define SSD1306_CMD_DISPLAY_ALL_ON          0xA5
#define SSD1306_CMD_SET_COLUMN_ADDRESS      0x21
#define SSD1306_CMD_SET_PAGE_ADDRESS        0x22

//command parameters
#define SSD1306_ADDRESSING_HORIZONTAL       0x00
#define SSD1306_ADDRESSING_VERTICAL         0x01
#define SSD1306_MEMORY_PAGE                 0x02
#define SSD1306_CHARGE_PUMP_ENABLE          0x14
#define SSD1306_CHARGE_PUMP_DISABLE         0x10
#define SSD1306_COM_CONFIG_SEQUENTIAL       0x02
#define SSD1306_COM_CONFIG_ALTERNATE        0x12
#define SSD1306_PRECHARGE_PERIOD_DEFAULT    0xF1
#define SSD1306_DISPLAY_CLOCK_DEFAULT       0x80
#define SSD1306_MUX_64                      0x3F









#endif /* SSD1306_DISPLAY_INC_SSD1306_H_ */
