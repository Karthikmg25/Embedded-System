/*
 * SSD1306_Graphics.h
 *
 *  Created on: Aug 31, 2026
 *      Author: hp
 */

#ifndef SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_
#define SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_

#include"SSD1306.h"
#include "GraphicsBitmap.h"

#include <stdlib.h>

//define pixel type enum
typedef enum
{
	GFX_PIXEL_OFF,
	GFX_PIXEL_ON

}GFXPixel_t;


//define graphics structure (later)

//define types for x,y coordinates
typedef uint8_t x_position_t;
typedef uint8_t y_position_t;

//declare an API to clear the frame buffer
void GFX_Clear(SSD1306_t *oled);

//declare an API to fill the display
void GFX_Fill(SSD1306_t *oled);

//declare API for drawing pixel with x,y coordianates
OLED_Status_t GFX_DrawPixel(SSD1306_t *oled, x_position_t x, y_position_t y, GFXPixel_t state);

//declare an API to check state of a pixel
GFXPixel_t GFX_GetPixel(SSD1306_t *oled, x_position_t x, y_position_t y);

//declare APIs for drawing lines

OLED_Status_t GFX_DrawHorizontalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state );
OLED_Status_t GFX_DrawVerticalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state );
OLED_Status_t GFX_DrawLine(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, GFXPixel_t state);

//declare API for drawing shapes

OLED_Status_t GFX_DrawRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state );
OLED_Status_t GFX_FillRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state );
OLED_Status_t GFX_DrawTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state);
OLED_Status_t GFX_FillTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state);
OLED_Status_t GFX_DrawCircle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t radius, GFXPixel_t state);
OLED_Status_t GFX_FillCircle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t radius, GFXPixel_t state);

//declare API to draw bitmap
OLED_Status_t GFX_DrawBitmap(SSD1306_t *oled,x_position_t x,y_position_t y,const uint8_t *bitmap,uint8_t width,uint8_t height,GFXPixel_t state);

//declare APIs to draw character, strings and numbers

OLED_Status_t GFX_DrawChar(SSD1306_t *oled, char ch, column col, page_number page);
OLED_Status_t GFX_DrawString(SSD1306_t *oled, char *str, column col, page_number page);
OLED_Status_t GFX_DrawInt(SSD1306_t *oled, int num, column col, page_number page);
OLED_Status_t GFX_DrawFloat(SSD1306_t *oled, float num,uint8_t decimals, column col, page_number page);

#endif /* SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_ */
