/*
 * SSD1306_Graphics.c
 *
 *  Created on: Aug 31, 2026
 *      Author: hp
 */
#include "SSD1306_Graphics.h"

void GFX_Clear(SSD1306_t *oled)
{
	// Reset all pixels to 0
	for(uint16_t i=0;i<SSD1306_FRAME_SIZE;i++)
	{
		oled->frame_buffer[i] = 0;
	}
}
void GFX_Fill(SSD1306_t *oled)
{
	// Set all pixels to 1 : set byte as 1111 1111
	for(uint16_t i=0;i<SSD1306_FRAME_SIZE;i++)
	{
		oled->frame_buffer[i] = 0xFF;
	}
}
//define API for drawing pixel with x,y coordianates
OLED_Status_t GFX_DrawPixel(SSD1306_t *oled, x_position_t x, y_position_t y, GFXPixel_t state)
{
	//validate input
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}
	//find page number and bit position of the pixel
	uint8_t page = y / 8;
	uint8_t bit  = y % 8;

	//find frame buffer byte
	uint16_t byte_index = (page * SSD1306_WIDTH )+ x ;

	if(state == GFX_PIXEL_ON)
	{
		//set the bit
		oled->frame_buffer[byte_index] |= (1<< bit);
	}
	else if(state == GFX_PIXEL_OFF)
	{
		//clear the bit
		oled->frame_buffer[byte_index] &=~(1<< bit);
	}

	return OLED_OKAY;
}

OLED_Status_t GFX_DrawHorizontalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state )
{
	//perform validation
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}
	if((uint16_t)x+length > SSD1306_WIDTH)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}
	//draw pixels starting from x to x + length
	for(uint16_t i= x;i< (x + length);i++)
	{
		GFX_DrawPixel(oled, i, y, state);
	}
	return OLED_OKAY;
}
OLED_Status_t GFX_DrawVerticalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state )
{
	//perform validation
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}
	if((uint16_t)y+length > SSD1306_HEIGHT)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}
	//draw pixels from y to y + length
	for(uint16_t i=y;i< (y+length);i++)
	{
		GFX_DrawPixel(oled, x, i, state);
	}

	return OLED_OKAY;
}
OLED_Status_t GFX_DrawRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state )
{
	// prform validation first:
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}
	if((uint16_t)x + length > SSD1306_WIDTH ||
	   (uint16_t)y + height > SSD1306_HEIGHT 	)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}

	//draw 4 lines connecting 4 coordinates:
	// - horizontal line : (x,y)           with length
	// - horizontal line : (x,y+height-1)  with length
	// - vertical line   : (x,y)           with height
	// - vertical line   : (x+length-1)    with height
                                                                                // assume length = 50, height = 20
	GFX_DrawHorizontalLine(oled, x, y, length, state);            //Top          (20, 30) (69, 30)
	GFX_DrawHorizontalLine(oled, x, (y+height-1), length, state); //Bottom       (20, 49) (69, 49)
	GFX_DrawVerticalLine(oled, x, y, height, state);              //Left         (20, 30) (20, 49)
	GFX_DrawVerticalLine(oled,(x+length-1), y, height, state);    //Right        (69, 30) (69, 49)

	return OLED_OKAY;
}

OLED_Status_t GFX_FillRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state )
{
	//perform validation first:
		if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
		{
			return OLED_ERROR_INVALID_COORDINATE;
		}
		if((uint16_t)x + length > SSD1306_WIDTH ||
		   (uint16_t)y + height > SSD1306_HEIGHT 	)
		{
			return OLED_ERROR_BUFFER_OVERFLOW;
		}

	//this means deawing a set of horizontal lines form y to (y+height-1)
	// - from (x,y)          (x+length-1,y)
	//   to   (x,y+height-1) (x+length-1, y+height-1)
	for(uint8_t i=y;i< y+height;i++)
	{
		GFX_DrawHorizontalLine(oled, x, i, length, state);
	}

	return OLED_OKAY;
}

OLED_Status_t GFX_DrawLine(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, GFXPixel_t state)
{
	// perform validations

	//draw first pixel
	GFX_DrawPixel(oled, x0, y0, state);

	// find next pixels

	// Use Bresenham's Algorithm:
	//
	// - first check the slope: if (dy > dx) or not
	// - calculate decision parameter (pk) depending on the slope
	// - calculate the next pixel based on the decision parameter
	// - update pk properly for finding next pixel
	// - calculate pixels till end point

	int16_t pk;// decision parameter
	int16_t dy = y1-y0;
	int16_t dx = x1-x0;

	//check slope
	if(dx > dy)         // m < 1 : we primarily increment x and decides whether y increments or not based on pk.
	{

		// 1. find value of pk
		pk = 2*dy - dx;

		while(x0 < x1)
		{

			// 2. decide value of y
			if(pk < 0)
			{
				// y stays same

				// 3. update pk for the next pixel
				pk = pk + (2*dy);
			}
			else //if(pk >=0)
			{
				y0++;// increment y

				// 3. update pk for the next pixel
				pk = pk + (2*dy) - (2*dx);
			}

			x0++;// increment x

			//draw the updated pixel
			GFX_DrawPixel(oled, x0, y0, state);
		}
	}
	if(dy > dx) // m >= 1: we primaritly increment y and decides whether x statys same or not based on pk.
	{

		// 1. find value of pk
		pk = 2*dx - dy;

		while(y0 < y1)
		{

			// 2. decide value of x
			if(pk < 0)
			{
				// x stays same

				// 3. Update pk
				pk = pk+ 2*dx;
			}
			else
			{
				//increment x
				x0++;

				// 3. Update pk
				pk = pk+ 2*dx - (2*dy);

			}


			//increment y
			y0++;

			//draw the updated pixel
			GFX_DrawPixel(oled, x0, y0, state);


		}
                              //!!!!!!!! this version is working, but only when x and y increments ( line in right/down direction)
	}
	return OLED_OKAY;
}

















