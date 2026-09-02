/*
 * SSD1306.c
 *
 *  Created on: Aug 24, 2026
 *      Author: hp
 */

#include "SSD1306.h"

// define font array for printing characters
const uint8_t font_array[96][5] = {            // 5x7 ASCII font array,96 characters ASCII 32–127
	{0x00,0x00,0x00,0x00,0x00}, // ' ' 32
	{0x00,0x00,0x5F,0x00,0x00}, // '!' 33     ___
	{0x00,0x07,0x00,0x07,0x00}, // '"' 34       |
	{0x14,0x7F,0x14,0x7F,0x14}, // '#' 35       |
	{0x24,0x2A,0x7F,0x2A,0x12}, // '$' 36       |__
	{0x23,0x13,0x08,0x64,0x62}, // '%' 37        __  ASCII values : row 0 means ASCII 32
	{0x36,0x49,0x55,0x22,0x50}, // '&' 38       |
	{0x00,0x05,0x03,0x00,0x00}, // ''' 39       |
	{0x00,0x1C,0x22,0x41,0x00}, // '(' 40       |
	{0x00,0x41,0x22,0x1C,0x00}, // ')' 41
	{0x14,0x08,0x3E,0x08,0x14}, // '*' 42
	{0x08,0x08,0x3E,0x08,0x08}, // '+' 43
	{0x00,0x50,0x30,0x00,0x00}, // ',' 44
	{0x08,0x08,0x08,0x08,0x08}, // '-' 45
	{0x00,0x60,0x60,0x00,0x00}, // '.' 46
	{0x20,0x10,0x08,0x04,0x02}, // '/' 47
	{0x3E,0x51,0x49,0x45,0x3E}, // '0' 48
	{0x00,0x42,0x7F,0x40,0x00}, // '1' 49
	{0x42,0x61,0x51,0x49,0x46}, // '2' 50
	{0x21,0x41,0x45,0x4B,0x31}, // '3' 51
	{0x18,0x14,0x12,0x7F,0x10}, // '4' 52
	{0x27,0x45,0x45,0x45,0x39}, // '5' 53
	{0x3C,0x4A,0x49,0x49,0x30}, // '6' 54
	{0x01,0x71,0x09,0x05,0x03}, // '7' 55
	{0x36,0x49,0x49,0x49,0x36}, // '8' 56
	{0x06,0x49,0x49,0x29,0x1E}, // '9' 57
	{0x00,0x36,0x36,0x00,0x00}, // ':' 58
	{0x00,0x56,0x36,0x00,0x00}, // ';' 59
	{0x08,0x14,0x22,0x41,0x00}, // '<' 60
	{0x14,0x14,0x14,0x14,0x14}, // '=' 61
	{0x00,0x41,0x22,0x14,0x08}, // '>' 62
	{0x02,0x01,0x51,0x09,0x06}, // '?' 63
	{0x32,0x49,0x79,0x41,0x3E}, // '@' 64
	{0x7E,0x11,0x11,0x11,0x7E}, // 'A' 65
	{0x7F,0x49,0x49,0x49,0x36}, // 'B' 66
	{0x3E,0x41,0x41,0x41,0x22}, // 'C' 67
	{0x7F,0x41,0x41,0x22,0x1C}, // 'D' 68
	{0x7F,0x49,0x49,0x49,0x41}, // 'E' 69
	{0x7F,0x09,0x09,0x09,0x01}, // 'F' 70
	{0x3E,0x41,0x49,0x49,0x7A}, // 'G' 71
	{0x7F,0x08,0x08,0x08,0x7F}, // 'H' 72
	{0x00,0x41,0x7F,0x41,0x00}, // 'I' 73
	{0x20,0x40,0x41,0x3F,0x01}, // 'J' 74
	{0x7F,0x08,0x14,0x22,0x41}, // 'K' 75
	{0x7F,0x40,0x40,0x40,0x40}, // 'L' 76
	{0x7F,0x02,0x0C,0x02,0x7F}, // 'M' 77
	{0x7F,0x04,0x08,0x10,0x7F}, // 'N' 78
	{0x3E,0x41,0x41,0x41,0x3E}, // 'O' 79
	{0x7F,0x09,0x09,0x09,0x06}, // 'P' 80
	{0x3E,0x41,0x51,0x21,0x5E}, // 'Q' 81
	{0x7F,0x09,0x19,0x29,0x46}, // 'R' 82
	{0x46,0x49,0x49,0x49,0x31}, // 'S' 83
	{0x01,0x01,0x7F,0x01,0x01}, // 'T' 84
	{0x3F,0x40,0x40,0x40,0x3F}, // 'U' 85
	{0x1F,0x20,0x40,0x20,0x1F}, // 'V' 86
	{0x3F,0x40,0x38,0x40,0x3F}, // 'W' 87
	{0x63,0x14,0x08,0x14,0x63}, // 'X' 88
	{0x07,0x08,0x70,0x08,0x07}, // 'Y' 89
	{0x61,0x51,0x49,0x45,0x43}, // 'Z' 90
	{0x00,0x7F,0x41,0x41,0x00}, // '[' 91
	{0x02,0x04,0x08,0x10,0x20}, // '\' 92
	{0x00,0x41,0x41,0x7F,0x00}, // ']' 93
	{0x04,0x02,0x01,0x02,0x04}, // '^' 94
	{0x40,0x40,0x40,0x40,0x40}, // '_' 95
	{0x00,0x01,0x02,0x04,0x00}, // '' 96
	{0x20,0x54,0x54,0x54,0x78}, // 'a' 97
	{0x7F,0x48,0x44,0x44,0x38}, // 'b' 98
	{0x38,0x44,0x44,0x44,0x20}, // 'c' 99
	{0x38,0x44,0x44,0x48,0x7F}, // 'd'100
	{0x38,0x54,0x54,0x54,0x18}, // 'e'101
	{0x08,0x7E,0x09,0x01,0x02}, // 'f'102
	{0x0C,0x52,0x52,0x52,0x3E}, // 'g'103
	{0x7F,0x08,0x04,0x04,0x78}, // 'h'104
	{0x00,0x44,0x7D,0x40,0x00}, // 'i'105
	{0x20,0x40,0x44,0x3D,0x00}, // 'j'106
	{0x7F,0x10,0x28,0x44,0x00}, // 'k'107
	{0x00,0x41,0x7F,0x40,0x00}, // 'l'108
	{0x7C,0x04,0x18,0x04,0x78}, // 'm'109
	{0x7C,0x08,0x04,0x04,0x78}, // 'n'110
	{0x38,0x44,0x44,0x44,0x38}, // 'o'111
	{0x7C,0x14,0x14,0x14,0x08}, // 'p'112
	{0x08,0x14,0x14,0x18,0x7C}, // 'q'113
	{0x7C,0x08,0x04,0x04,0x08}, // 'r'114
	{0x48,0x54,0x54,0x54,0x20}, // 's'115
	{0x04,0x3F,0x44,0x40,0x20}, // 't'116
	{0x3C,0x40,0x40,0x20,0x7C}, // 'u'117
	{0x1C,0x20,0x40,0x20,0x1C}, // 'v'118
	{0x3C,0x40,0x30,0x40,0x3C}, // 'w'119
	{0x44,0x28,0x10,0x28,0x44}, // 'x'120
	{0x0C,0x50,0x50,0x50,0x3C}, // 'y'121
	{0x44,0x64,0x54,0x4C,0x44}, // 'z'122
	{0x00,0x08,0x36,0x41,0x00}, // '{'123
	{0x00,0x00,0x7F,0x00,0x00}, // '|'124
	{0x00,0x41,0x36,0x08,0x00}, // '}'125
	{0x02,0x01,0x02,0x04,0x02}, // '~'126
	{0x00,0x00,0x00,0x00,0x00}  // DEL127 (optional)
};

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
static void SSD1306_SendData(SSD1306_t *oled,uint8_t data)
{
	I2C_Transmit_Byte(oled->I2C_interface, OLED_ADDRESS, SSD1306_DATA, data);
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
OLED_Status_t SSD1306_PrintChar(SSD1306_t *oled, char ch, column col, page_number page)
{
	//validation of user input :page,column and character
	if(ch < 32 || ch > 127)
	{
		return OLED_ERROR_INVALID_CHARACTER;
	}
	if(col >= SSD1306_WIDTH - 5)
	{
		return OLED_ERROR_INVALID_COLUMN;
	}
	if(page >=  SSD1306_PAGES)
	{
		return OLED_ERROR_INVALID_PAGE;
	}

	//modify 5 bytes of RAM buffer to add the character
	for(uint8_t i=0;i<5;i++)
	{
		oled->frame_buffer[page*SSD1306_WIDTH + col + i] = font_array[ch-32][i];
	}
	return OLED_OKAY;
}
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
OLED_Status_t SSD1306_PrintString(SSD1306_t *oled, char *str, column col, page_number page)
{
	//validation of user input : page,column and character

		if(col > SSD1306_WIDTH - 5)
		{
			return OLED_ERROR_INVALID_COLUMN;
		}
		if(page >= SSD1306_PAGES)
		{
			return OLED_ERROR_INVALID_PAGE;
		}


	//modify frame buffer
	while(*str)
	{
		char ch = *str;
		if(ch < 32 || ch > 127)
		{
			return OLED_ERROR_INVALID_CHARACTER;
		}
		//if column overflows 127, print on next page
		if(col >  SSD1306_WIDTH - 5)
		{
			page= page+1;
			col = 0;
			if(page >=  SSD1306_PAGES)
			{
				return OLED_ERROR_INVALID_PAGE;
			}
		}


		//modify 5 bytes for one character
		for(uint8_t i=0;i<5;i++)
		{
			oled->frame_buffer[page*SSD1306_WIDTH + col + i] = font_array[ch - 32][i];
		}
		//before printing next character, leave some space
		col = col + 7;

		//move to next character
		str++;
	}

	return OLED_OKAY;
}
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
OLED_Status_t SSD1306_PrintInt(SSD1306_t *oled, int num, column col, page_number page)
{
	char buffer[12];// declare a buffer to store string

	snprintf(buffer,sizeof(buffer), "%ld", (long)num);

	return SSD1306_PrintString(oled, buffer, col, page);
}
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
OLED_Status_t SSD1306_PrintFloat(SSD1306_t *oled, float num,uint8_t decimals, column col, page_number page)
{
	char buffer[20];// declare a buffer to store string

	snprintf(buffer, sizeof(buffer), "%.*f", decimals, num);

	return SSD1306_PrintString(oled, buffer, col, page);
}
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





