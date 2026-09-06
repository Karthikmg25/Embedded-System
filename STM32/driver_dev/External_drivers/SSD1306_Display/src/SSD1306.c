/*
 * SSD1306.c
 *
 *  Created on: Aug 24, 2026
 *      Author: hp
 */

#include "SSD1306.h"

// OLED helper functions
/**********************************************************************************************************************/
static void SSD1306_SendCommand(SSD1306_t *oled, uint8_t cmd)
{
	I2C_Transmit_Byte(oled->I2C_interface, OLED_ADDRESS, SSD1306_COMMAND, cmd);
}
static void SSD1306_SendCommandBuffer(SSD1306_t *oled, uint8_t *buff, uint8_t len)
{
	I2C_Transmit_Buffer(oled->I2C_interface, OLED_ADDRESS, SSD1306_COMMAND, buff, len);
}

static void SSD1306_SendDataBuffer(SSD1306_t *oled,uint8_t *buff, uint16_t len)
{
	I2C_Transmit_Buffer(oled->I2C_interface, OLED_ADDRESS, SSD1306_DATA, buff, len);
}
static OLED_Status_t SSD1306_SetOrientation(SSD1306_t *oled)
{
	switch(oled->orientation)
	{
		case OLED_ORIENTATION_NORMAL         :  SSD1306_SendCommand(oled, SSD1306_CMD_SET_SEGMENT_REMAP);
										        SSD1306_SendCommand(oled, SSD1306_CMD_SET_COM_SCAN_NORMAL);
										        break;
		case OLED_ORIENTATION_HORIZONTAL_FLIP: 	SSD1306_SendCommand(oled, SSD1306_CMD_SET_SEGMENT_REMAP_INV);
		                                        SSD1306_SendCommand(oled, SSD1306_CMD_SET_COM_SCAN_NORMAL);
		                                        break;
		case OLED_ORIENTATION_VERTICAL_FLIP  : 	SSD1306_SendCommand(oled, SSD1306_CMD_SET_SEGMENT_REMAP);
				                                SSD1306_SendCommand(oled, SSD1306_CMD_SET_COM_SCAN_REVERSE);
				                                break;

		case OLED_ORIENTATION_ROTATE180      : 	SSD1306_SendCommand(oled, SSD1306_CMD_SET_SEGMENT_REMAP_INV);
				                                SSD1306_SendCommand(oled, SSD1306_CMD_SET_COM_SCAN_REVERSE);
				                                break;
		default                              :  return OLED_ERROR_INVALID_ORIENTATION;
	}
	return OLED_OKAY;
}

//define public APIs
/**********************************************************************************************************************/
/**
 * @brief:  Initializes the SSD1306 OLED display.
 *
 * 			Configures the SSD1306 controller for a 128x64 monochrome display,
 * 			sets the display orientation and initial contrast, and finally
 * 			enables the display.
 *
 * @param:  oled Pointer to the SSD1306 device structure.
 *
 * @return: OLED_OKAY if initialization is successful.
 * @return: OLED_ERROR_INVALID_ORIENTATION if the configured orientation
 *          is invalid.
 */
OLED_Status_t SSD1306_Init(SSD1306_t *oled)
{
	// Required controller configuration:
	// The display is kept OFF while the controller is configured.

	uint8_t init_commands[] = {
			                                     /* Initialization sequence */

			SSD1306_CMD_DISPLAY_OFF,                                           // display OFF before settings

			SSD1306_CMD_SET_CHARGE_PUMP, SSD1306_CHARGE_PUMP_ENABLE,           // enable internal charge pump

			SSD1306_CMD_SET_COM_PINS_CONFIG, SSD1306_COM_CONFIG_ALTERNATE,     // COM configuration for 64 row display (Alternate configuration)

			SSD1306_CMD_SET_MULTIPLEX, SSD1306_MUX_64 ,                        // Scan 64 COM lines
			                                                                   // (Optional as it is 0x3F by default)
			SSD1306_CMD_SET_PRECHARGE_PERIOD,SSD1306_PRECHARGE_PERIOD_DEFAULT, // set charge period as 15 clock cycles

			SSD1306_CMD_SET_ADDRESSING_MODE,SSD1306_ADDRESSING_HORIZONTAL,     // set horizontal addressing mode

			SSD1306_CMD_DISPLAY_RAM,                                           // display what is on RAM buffer

	};
	SSD1306_SendCommandBuffer(oled, init_commands, sizeof( init_commands));


	if(SSD1306_SetOrientation(oled) != OLED_OKAY)
	{                                                                            //set orientaion configured in the device structure
		return OLED_ERROR_INVALID_ORIENTATION;
	}

	                                                                             //set initial contrast configured in the device structure
	SSD1306_SetContrast(oled, oled->contrast);

	SSD1306_SendCommand(oled, SSD1306_CMD_DISPLAY_ON);                           //Enable OLED display output

	return OLED_OKAY;

}
/**
 * @brief:		 Writes a single character to the OLED framebuffer.
 *
 * 				Renders a character using the built-in 5x7 font and stores the
 * 				corresponding pixel data in the framebuffer. The display is not
 * 				updated until SSD1306_Update() is called.
 *
 * @param:		 oled - Pointer to the SSD1306 device structure.
 * @param:		 ch   - Character to be displayed (ASCII 32 to 127).
 * @param:		 col  - Starting column position (0 to 123).
 * @param:		 page - Page position (0 to 7).
 *
 * @return: 	 OLED_OKAY if the character is written successfully.
 * @return:		 OLED_ERROR_INVALID_CHARACTER if the character is outside
 *         			the supported ASCII range.
 * @return:		 OLED_ERROR_INVALID_COLUMN if the character would exceed
 *         			the display width.
 * @return:		 OLED_ERROR_INVALID_PAGE if the specified page is invalid.
 */

/**
 * @brief:	 Updates the OLED display with the contents of the framebuffer.
 *
 * 				Sets the SSD1306 column and page address ranges to cover the
 * 				entire 128x64 display, then transfers the complete framebuffer
 * 				to the display RAM.
 *
 * @param:  oled - Pointer to the SSD1306 device structure.
 *
 * @return: none
 */
void SSD1306_Update(SSD1306_t *oled)
{
	//set column and row range (128*64)
			uint8_t addr_cmds[] = {
					SSD1306_CMD_SET_COLUMN_ADDRESS , 0, SSD1306_WIDTH-1,   // set column address : 0-127
					SSD1306_CMD_SET_PAGE_ADDRESS   , 0, SSD1306_PAGES-1    // set page address   : 0-7
			};
			SSD1306_SendCommandBuffer(oled, addr_cmds, sizeof(addr_cmds));

			//send frame buffer to print the string
			SSD1306_SendDataBuffer(oled, oled->frame_buffer,  SSD1306_FRAME_SIZE);
}
/**
 * @brief:	 Clears the OLED framebuffer.
 *
 * 				Sets all pixels in the framebuffer to OFF. The physical display
 * 				is not updated until SSD1306_Update() is called.
 *
 * @param:  oled - Pointer to the SSD1306 device structure.
 *
 * @return: none
 */
void SSD1306_ClearDisplay(SSD1306_t *oled)
{
	//modify  RAM buffer : set all pixels as zero
	for(uint16_t i=0;i< SSD1306_FRAME_SIZE;i++)
	{
		oled->frame_buffer[i] = 0;
	}

	//SSD1306_Update();

}
/**
 * @brief:	 Writes a string to the OLED framebuffer.
 *
 * 				Renders each character using the built-in 5x7 font and stores
 * 				the resulting pixel data in the framebuffer. A 2-column space
 * 				is left between consecutive characters. When the remaining
 * 				space on a page is insufficient for the next character, text
 * 				continues from the beginning of the next page.
 *
 * @param:  oled - Pointer to the SSD1306 device structure.
 * @param:  str  - Pointer to the null-terminated string.
 * @param:  col  - Starting column position.
 * @param:  page - Starting page position.
 *
 * @return:  OLED_OKAY if the string is written successfully.
 * @return:  OLED_ERROR_INVALID_COLUMN if the starting column is invalid.
 * @return:  OLED_ERROR_INVALID_PAGE if the starting or next page is invalid.
 * @return:  OLED_ERROR_INVALID_CHARACTER if an unsupported character
 *           is encountered.
 */

/**
 * @brief:	 Writes a signed integer to the OLED framebuffer.
 *
 * 				Converts the integer to a character string and renders it using
 * 				the SSD1306_PrintString() API.
 *
 * @param:  oled - Pointer to the SSD1306 device structure.
 * @param:  num  - Integer value to be displayed.
 * @param:  col  - Starting column position.
 * @param:  page - Starting page position.
 *
 * @return: Status returned by SSD1306_PrintString().
 */


/**
 * @brief:  Writes a floating-point value to the OLED framebuffer.
 *
 * 				Converts the floating-point value to a character string with the
 * 				requested number of decimal places and renders it using
 * 				SSD1306_PrintString().
 *
 * @param: oled     - Pointer to the SSD1306 device structure.
 * @param: num      - Floating-point value to be displayed.
 * @param: decimals - Number of digits to display after the decimal point.
 * @param: col      - Starting column position.
 * @param: page     - Starting page position.
 *
 * @return Status returned by SSD1306_PrintString().
 */

/**
 * @brief Sets the display contrast.
 *
 * 			Sends the contrast value to the SSD1306 controller and updates
 * 			the contrast value stored in the device structure.
 *
 * @param oled Pointer to the SSD1306 device structure.
 * @param contrast Contrast value (0x00 to 0xFF).
 */
void SSD1306_SetContrast(SSD1306_t *oled, uint8_t contrast)
{
	SSD1306_SendCommand(oled, SSD1306_CMD_SET_CONTRAST);
	SSD1306_SendCommand(oled, contrast);

	//update contrast
	oled->contrast = contrast;
}
/**
 * @brief Sets the display inversion mode.
 *
 * 			When inversion is enabled, pixels that are normally ON become OFF
 * 			and pixels that are normally OFF become ON.
 *
 * @param oled Pointer to the SSD1306 device structure.
 * @param invert true to invert the display, false for normal display.
 */
void SSD1306_DisplayInvert(SSD1306_t *oled, bool invert )
{
	if(invert)
	{
		SSD1306_SendCommand(oled, SSD1306_CMD_DISPLAY_INVERTED);
	}
	else
	{
		SSD1306_SendCommand(oled, SSD1306_CMD_DISPLAY_NORMAL);
	}
}
/**
 * @brief Turns the OLED display output OFF.
 *
 * The display RAM contents are retained while the display is OFF.
 *
 * @param oled Pointer to the SSD1306 device structure.
 */
void SSD1306_DisplayOff(SSD1306_t *oled)
{
	SSD1306_SendCommand(oled, SSD1306_CMD_DISPLAY_OFF);
}
/**
 * @brief Turns the OLED display output ON.
 *
 * The previously stored display RAM contents are displayed.
 *
 * @param oled Pointer to the SSD1306 device structure.
 */
void SSD1306_DisplayOn(SSD1306_t *oled)
{
	SSD1306_SendCommand(oled, SSD1306_CMD_DISPLAY_ON);
}





