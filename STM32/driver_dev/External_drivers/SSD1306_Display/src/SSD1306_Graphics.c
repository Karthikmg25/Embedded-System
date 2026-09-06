/*
 * SSD1306_Graphics.c
 *
 *  Created on: Aug 31, 2026
 *      Author: hp
 */
#include "SSD1306_Graphics.h"

/**
 * @file SSD1306_Graphics.c
 * @brief Implementation of the graphics primitives layer.
 *        See SSD1306_Graphics.h for the full API documentation.
 */


// define font array for printing characters
const uint8_t Font_array[96][5] = {            // 5x7 ASCII font array,96 characters ASCII 32–127
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

//create an 8X8 bitmap for learning
const char bit_map[]={
    0x3C,              //row0
    0x42,              //row1
    0x81,              //row2
    0xBD,
    0xBD,
    0x81,
    0x42,
    0x3C               //row7
};
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

GFXPixel_t GFX_GetPixel(SSD1306_t *oled, x_position_t x, y_position_t y)
{
	// validate the coordinates
	if(x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}

	//find page number and bit position of the pixel
		uint8_t page = y / 8;
		uint8_t bit  = y % 8;

		//find frame buffer byte
		uint16_t byte_index = (page * SSD1306_WIDTH )+ x ;

		uint8_t byte = oled->frame_buffer[byte_index];

		//return state of pixel
		if(byte & (1<<bit))
		{
			return GFX_PIXEL_ON;
		}
		else
		{
			return GFX_PIXEL_OFF;
		}
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
	// perform validation
	if(x0 >= SSD1306_WIDTH || x1 >= SSD1306_WIDTH || y0 >= SSD1306_HEIGHT || y1 >= SSD1306_HEIGHT)
	{
				return OLED_ERROR_INVALID_COORDINATE;
	}


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



	// For implementing the algorithm in all quadrants,
	// - consider the magnitude of dy and dx and ignore it's sign (use of abs function )
	// - update the value of coordinates based on direction: not just barely increment it ( use of sx and sy )

	int16_t pk;                           // decision parameter
	int16_t dy = abs(y1-y0);              // ignore the sign while measuring slope, only take the magnitude of diffrence//
	int16_t dx = abs(x1-x0);

	// using sx and sy for updating the coordinates properly based on direction of line.

	int8_t sx,sy;

	if(x0 < x1) {sx =  1;} // Updation - incrementing
	else        {sx = -1;} // Updation - deccrementing

	if(y0 < y1) {sy =  1;} // Updation - incrementing
	else        {sy = -1;} // Updation - decrementing


	//check slope
	if(dx > dy)         // m < 1 : we primarily increment x and decides whether y increments or not based on pk.
	{

		// 1. find value of pk
		pk = 2*dy - dx;

		do
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
				y0 += sy;// update y

				// 3. update pk for the next pixel
				pk = pk + (2*dy) - (2*dx);
			}

			x0+=sx;// Always update x when m is < 1

			//draw the updated pixel
			GFX_DrawPixel(oled, x0, y0, state);

		}while(x0 != x1); // stop drawing when reaches the end point
	}
	else if(dy >= dx) // m >= 1: we primaritly increment y and decides whether x statys same or not based on pk.
	{                 // when m=1, we want to increment both x and y always, and algorithm handles this anuotmatically

		// 1. find value of pk
		pk = 2*dx - dy;

		do
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
				//update x
				x0+=sx;

				// 3. Update pk
				pk = pk+ 2*dx - (2*dy);

			}


			//Always update y when m is >=1
			y0+=sy;

			//draw the updated pixel
			GFX_DrawPixel(oled, x0, y0, state);


		}while(y0 != y1);// stop drawing when reaches the end point

	}
	return OLED_OKAY;
}


OLED_Status_t GFX_DrawTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state)
{

	//perform validation first:
	if(x0 >= SSD1306_WIDTH || x1 >= SSD1306_WIDTH || y0 >= SSD1306_HEIGHT || y1 >= SSD1306_HEIGHT
			|| x2 >= SSD1306_WIDTH || y2 >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}
	//draw 3 lines connecting the 3 coordinates to draw the triangle

	GFX_DrawLine(oled, x0, y0, x1, y1, state);
	GFX_DrawLine(oled, x1, y1, x2, y2, state);
	GFX_DrawLine(oled, x0, y0, x2, y2, state);

	return OLED_OKAY;
}
//define helper functions
static uint8_t smallest(uint8_t a, uint8_t b, uint8_t c)
{
	if(a <b && a<c)
	{
		return a;
	}
	else
	if(b<a && b<c)
	{
		return b;
	}
	else
		return c;
}
static uint8_t largest(uint8_t a, uint8_t b, uint8_t c)
{
	if(a > b && a > c)
	{
		return a;
	}
	else
	if(b > a && b > c)
	{
		return b;
	}
	else
		return c;
}
OLED_Status_t GFX_FillTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state)
{
	// validate the coordinates
	if(x0 >= SSD1306_WIDTH || x1 >= SSD1306_WIDTH || y0 >= SSD1306_HEIGHT || y1 >= SSD1306_HEIGHT
			|| x2 >= SSD1306_WIDTH || y2 >= SSD1306_HEIGHT)
	{
		return OLED_ERROR_INVALID_COORDINATE;
	}

	//first draw the triangle
	GFX_DrawTriangle(oled, x0, y0, x1, y1, x2, y2, state);

	//find top and bottom rows
	uint8_t y_top    = smallest(y0, y1, y2);
	uint8_t y_bottom = largest(y0, y1, y2);

	//loop through each rows and turn on pixels between two ON pixels

	uint8_t y_current = y_top;



	while(y_current <= y_bottom)
	{
		uint8_t flag=0, x_first, x_second;

		//loop through each columns (x values)
		for (uint8_t x_current = 0;x_current < SSD1306_WIDTH;x_current++)
		{
			if(GFX_GetPixel(oled, x_current, y_current) == state && flag == 0)
			{
				x_first = x_current;

				flag=1;
			}
			else
				if(GFX_GetPixel(oled, x_current, y_current) == state && flag == 1)
				{
					x_second = x_current;

					flag=0;

					//Draw line when second pixel is found
					//fill row between two x points
							GFX_DrawLine(oled, x_first, y_current, x_second, y_current, state);
				}

		}


		y_current++;
	}




	return OLED_OKAY;
}


OLED_Status_t GFX_DrawCircle(SSD1306_t *oled, x_position_t xc, y_position_t yc, uint8_t radius, GFXPixel_t state)
{
	//perform validations

	if(xc+radius >= SSD1306_WIDTH || yc+radius >= SSD1306_HEIGHT

			|| xc<radius || yc<radius)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}

	// Calculate pixels for one octant and use 8-way symmetry
	// to generate the 7 remaining points.
	//
	// For a point (x0,y0) relative to the center,the other symmetric points are:
	// ( x0, y0), ( x0,-y0), (-x0, y0), (-x0,-y0)
	// ( y0, x0), ( y0,-x0), (-y0, x0), (-y0,-x0)
	//
	// Assume the circle center is origin, start at the top of the circle: (0,radius).
	// x is incremented at every step.
	// The decision parameter determines whether y should also decrease.
	// Octant is complete when x0 equals y0
	// Convert local coordinates (x0,y0) to OLED coordinates
	// by adding the circle center (x,y).

	// set starting point as (0, radius)
	int8_t x0 = 0, y0 = radius;

	// Initialize decision parameter
	int16_t pk = 3 - 2*radius;

	do{

		//draw pixels on 8 coordinates: centre is at (x,y)
			GFX_DrawPixel(oled, xc+x0, yc+y0, state);
			GFX_DrawPixel(oled, xc+x0, yc-y0, state);
			GFX_DrawPixel(oled, xc-x0, yc+y0, state);
			GFX_DrawPixel(oled, xc-x0, yc-y0, state);

			GFX_DrawPixel(oled, xc+y0, yc+x0, state);
			GFX_DrawPixel(oled, xc-y0, yc+x0, state);
			GFX_DrawPixel(oled, xc+y0, yc-x0, state);
			GFX_DrawPixel(oled, xc-y0, yc-x0, state);

			//find next coordinate using decision parameter

		if(pk < 0)
			{
			    pk = pk + 4*x0 + 6; // Update pk for next point

				                   // y stays same
				x0++;	           // x always increments

			}
			else
				if(pk >= 0)
				{
					pk = pk + 4*(x0-y0) + 10; // Update pk for next point

					y0--;                   // y decrements
					x0++;                   // x always increments

				}

	}while(x0!=y0);// loop till octant is complete

	return OLED_OKAY;
}

OLED_Status_t GFX_FillCircle(SSD1306_t *oled, x_position_t xc, y_position_t yc, uint8_t radius, GFXPixel_t state)
{
	//perform validations

	if(xc+radius >= SSD1306_WIDTH || yc+radius >= SSD1306_HEIGHT
				|| xc<radius || yc<radius)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}

	// for FillCrcle, find each coordinates in the octant, therby getting 7 other points,
	// instaed of drawing pixels on 8 points, draw 4 lines connecting 2 points

	// set starting point as (0, radius)
		int8_t x0 = 0, y0 = radius;

		// Initialize decision parameter
		int16_t pk = 3 - 2*radius;

		do{

			//draw line b/w 2 coordinates: centre is at (xc,yc)
			GFX_DrawLine(oled, xc+x0, yc+y0, xc+x0, yc-y0, state);

			GFX_DrawLine(oled, xc-x0, yc+y0, xc-x0, yc-y0, state);

			GFX_DrawLine(oled, xc+y0, yc+x0, xc-y0, yc+x0, state);

			GFX_DrawLine(oled, xc+y0, yc-x0, xc-y0, yc-x0, state);


				//find next coordinate using decision parameter

			if(pk < 0)
				{
				    pk = pk + 4*x0 + 6; // Update pk for next point

					                   // y stays same
					x0++;	           // x always increments

				}
				else
					if(pk >= 0)
					{
						pk = pk + 4*(x0-y0) + 10; // Update pk for next point

						y0--;                   // y decrements
						x0++;                   // x always increments

					}

		}while(x0<=y0);// loop till octant is complete

		return OLED_OKAY;


}
/**
 * @brief Draw a monochrome bitmap onto the OLED frame buffer.
 *
 * Draws a 1-bit-per-pixel bitmap at the specified top-left position.
 * The bitmap is stored in row-major order with pixels encoded MSB-first.
 *
 * @param[in] oled      Pointer to the SSD1306 OLED device structure.
 * @param[in] x         X-coordinate of the bitmap's top-left corner.
 * @param[in] y         Y-coordinate of the bitmap's top-left corner.
 * @param[in] bitmap    Pointer to the bitmap data.
 * @param[in] width     Width of the bitmap in pixels.
 * @param[in] height    Height of the bitmap in pixels.
 * @param[in] state     Pixel state to use when drawing the bitmap.
 *
 * @return OLED_STATUS_OK
 *         Bitmap was drawn successfully.
 * @return OLED_STATUS_INVALID_PARAM
 *         Invalid parameter or bitmap dimensions.
 * @return OLED_STATUS_OUT_OF_RANGE
 *         Bitmap extends outside the display boundaries.
 *
 * @note Bitmap format:
 *       - 1 bit per pixel (1 BPP)
 *       - 1 = pixel ON, 0 = pixel OFF
 *       - Row-major order (top-to-bottom, left-to-right)
 *       - MSB-first within each byte
 *       - Each row is padded to a whole number of bytes
 *
 */

OLED_Status_t GFX_DrawBitmap(SSD1306_t *oled,x_position_t x,y_position_t y,const uint8_t *bitmap,uint8_t width,uint8_t height,GFXPixel_t state)
{
	//perform validations

	if(x+width >= SSD1306_WIDTH || y+height > SSD1306_HEIGHT)
	{
		return OLED_ERROR_BUFFER_OVERFLOW;
	}

	//start from the row 0 of bitmap
	uint16_t bit_row = 0;

	//find nuber of bytes per row
	uint8_t bytes_per_row = (width + 7) / 8;

	while(bit_row < height)// loop through each rows of bitmap
	{

		//start from column 0 of bitmap
		uint16_t bit_col = 0;

		while(bit_col < width)// loop though each columns in the row
		{
			// find byte index based on column
			uint16_t byte_index = (bit_row*bytes_per_row)+bit_col / 8;
			//find bit index based on column
			uint8_t bit_index  = 7 - (bit_col % 8);

			uint8_t byte = bitmap[byte_index];

			//check MSB of byte and draw pixel based on that
			if((byte&(1<<bit_index)))
			{
				GFX_DrawPixel(oled, bit_col+x, bit_row+y, GFX_PIXEL_ON);
			}
			else
			{
				GFX_DrawPixel(oled, bit_col+x, bit_row+y, GFX_PIXEL_OFF);
			}

			bit_col++;

		}
		bit_row++;
	}
	return OLED_OKAY;
}
OLED_Status_t GFX_DrawChar(SSD1306_t *oled, char ch, column col, page_number page)
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
		oled->frame_buffer[page*SSD1306_WIDTH + col + i] = Font_array[ch-32][i];
	}
	return OLED_OKAY;
}
OLED_Status_t GFX_DrawString(SSD1306_t *oled, char *str, column col, page_number page)
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
			oled->frame_buffer[page*SSD1306_WIDTH + col + i] = Font_array[ch - 32][i];
		}
		//before printing next character, leave some space
		col = col + 7;

		//move to next character
		str++;
	}

	return OLED_OKAY;
}
OLED_Status_t GFX_DrawInt(SSD1306_t *oled, int num, column col, page_number page)
{
	char buffer[12];// declare a buffer to store string

	snprintf(buffer,sizeof(buffer), "%ld", (long)num);

	return GFX_DrawString(oled, buffer, col, page);
}
OLED_Status_t GFX_DrawFloat(SSD1306_t *oled, float num,uint8_t decimals, column col, page_number page)
{
	char buffer[20];// declare a buffer to store string

	snprintf(buffer, sizeof(buffer), "%.*f", decimals, num);

	return GFX_DrawString(oled, buffer, col, page);
}


















