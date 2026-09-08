/*
 * SSD1306_Graphics.h
 *
 *  Created on: Aug 31, 2026
 *      Author: hp
 */

#ifndef SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_
#define SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_


/**
 * @file SSD1306_Graphics.h
 * @brief Graphics primitives layer for the SSD1306 driver: pixels, lines,
 *        shapes (rectangle/triangle/circle, outline and filled), 1bpp
 *        bitmaps, and text (5x7 font) rendering.
 *
 * All functions here operate on the framebuffer inside the SSD1306_t
 * passed to them (see SSD1306.h) - none of them talk to the display
 * directly. After any sequence of GFX_* calls, call SSD1306_Update() to
 * push the framebuffer to the physical display.
 *
 * Coordinate system: (0,0) is the top-left pixel. X range is
 * 0-(SSD1306_WIDTH-1), Y range is 0-(SSD1306_HEIGHT-1).
 *
 */

#include"SSD1306.h"
#include "GraphicsBitmap.h"

#include <stdlib.h>


/**
 * @brief Logical pixel state used by every GFX_Draw GFX_Fill* API.
 */

typedef enum
{
	GFX_PIXEL_OFF,
	GFX_PIXEL_ON

}GFXPixel_t;


//define graphics structure (later)

//define types for x,y coordinates
typedef uint8_t x_position_t;
typedef uint8_t y_position_t;

/**
 * @brief   Clears the entire framebuffer (all pixels OFF).
 *
 *          Equivalent to SSD1306_ClearDisplay() - provided here as well so
 *          callers only using the GFX_* API don't need to reach into the
 *          lower-level driver. Call SSD1306_Update() afterwards to push the
 *          change to the physical display.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @return none
 */
void GFX_Clear(SSD1306_t *oled);

/**
 * @brief   Sets every pixel in the framebuffer ON.
 *
 *          Call SSD1306_Update() afterwards to push the change to the
 *          physical display.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @return none
 */
void GFX_Fill(SSD1306_t *oled);

/**
 * @brief   Sets or clears a single pixel in the framebuffer.
 *
 *          This is the primitive that every other GFX_Draw GFX_Fill*
 *          function is built on.
 *
 * @param[in,out] oled   Pointer to the SSD1306 device structure.
 * @param[in]     x      X coordinate (0-127).
 * @param[in]     y      Y coordinate (0-63).
 * @param[in]     state  GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Pixel written successfully.
 * @return OLED_ERROR_INVALID_COORDINATE x or y is outside the display bounds.
 */
OLED_Status_t GFX_DrawPixel(SSD1306_t *oled, x_position_t x, y_position_t y, GFXPixel_t state);

/**
 * @brief   Reads back the current state of a single pixel in the framebuffer.
 *
 * @param[in] oled  Pointer to the SSD1306 device structure.
 * @param[in] x     X coordinate (0-127).
 * @param[in] y     Y coordinate (0-63).
 *
 * @return GFX_PIXEL_ON or GFX_PIXEL_OFF reflecting the pixel's current state.
 * @note   If x/y is out of bounds this returns OLED_ERROR_INVALID_COORDINATE
 *         cast to GFXPixel_t - callers passing coordinates that are not
 *         already known-valid should range-check before relying on the
 *         returned value.
 */
GFXPixel_t GFX_GetPixel(SSD1306_t *oled, x_position_t x, y_position_t y);

//declare APIs for drawing lines

/**
 * @brief   Draws a horizontal line of pixels starting at (x, y).
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       Starting X coordinate (left end of the line).
 * @param[in]     y       Y coordinate (row) of the line.
 * @param[in]     length  Number of pixels to draw, extending in +X direction.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Line drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE x or y is outside the display bounds.
 * @return OLED_ERROR_BUFFER_OVERFLOW    x + length extends past the display edge.
 */
OLED_Status_t GFX_DrawHorizontalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state );
/**
 * @brief   Draws a vertical line of pixels starting at (x, y).
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       X coordinate (column) of the line.
 * @param[in]     y       Starting Y coordinate (top end of the line).
 * @param[in]     length  Number of pixels to draw, extending in +Y direction.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Line drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE x or y is outside the display bounds.
 * @return OLED_ERROR_BUFFER_OVERFLOW    y + length extends past the display edge.
 */
OLED_Status_t GFX_DrawVerticalLine(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, GFXPixel_t state );

/**
 * @brief   Draws an arbitrary line from (x0, y0) to (x1, y1).
 *
 *          Implemented with Bresenham's line algorithm, so it handles all
 *          slopes/quadrants without floating point.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     x0    Starting X coordinate.
 * @param[in]     y0    Starting Y coordinate.
 * @param[in]     x1    Ending X coordinate.
 * @param[in]     y1    Ending Y coordinate.
 * @param[in]     state GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Line drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE Any of x0/y0/x1/y1 is outside the display bounds.
 */
OLED_Status_t GFX_DrawLine(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, GFXPixel_t state);

//declare API for drawing shapes

/**
 * @brief   Draws an unfilled rectangle outline.
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       X coordinate of the top-left corner.
 * @param[in]     y       Y coordinate of the top-left corner.
 * @param[in]     length  Width of the rectangle in pixels.
 * @param[in]     height  Height of the rectangle in pixels.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Rectangle drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE x or y is outside the display bounds.
 * @return OLED_ERROR_BUFFER_OVERFLOW    Rectangle extends past the display edge.
 */
OLED_Status_t GFX_DrawRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state );

/**
 * @brief   Draws a filled (solid) rectangle.
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       X coordinate of the top-left corner.
 * @param[in]     y       Y coordinate of the top-left corner.
 * @param[in]     length  Width of the rectangle in pixels.
 * @param[in]     height  Height of the rectangle in pixels.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Rectangle drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE x or y is outside the display bounds.
 * @return OLED_ERROR_BUFFER_OVERFLOW    Rectangle extends past the display edge.
 */
OLED_Status_t GFX_FillRectangle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t length, uint8_t height, GFXPixel_t state );
/**
 * @brief   Draws a triangle outline connecting 3 vertices.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     x0    X coordinate of vertex 0.
 * @param[in]     y0    Y coordinate of vertex 0.
 * @param[in]     x1    X coordinate of vertex 1.
 * @param[in]     y1    Y coordinate of vertex 1.
 * @param[in]     x2    X coordinate of vertex 2.
 * @param[in]     y2    Y coordinate of vertex 2.
 * @param[in]     state GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Triangle drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE Any vertex is outside the display bounds.
 */
OLED_Status_t GFX_DrawTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state);

/**
 * @brief   Draws a filled (solid) triangle connecting 3 vertices.
 *
 *          Draws the outline first, then scans each row between the
 *          topmost and bottommost vertex, filling between the first and
 *          second outline pixel found on that row.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     x0    X coordinate of vertex 0.
 * @param[in]     y0    Y coordinate of vertex 0.
 * @param[in]     x1    X coordinate of vertex 1.
 * @param[in]     y1    Y coordinate of vertex 1.
 * @param[in]     x2    X coordinate of vertex 2.
 * @param[in]     y2    Y coordinate of vertex 2.
 * @param[in]     state GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                     Triangle drawn successfully.
 * @return OLED_ERROR_INVALID_COORDINATE Any vertex is outside the display bounds.
 *
 * @note Filling relies on GFX_GetPixel() matching @p state on the outline,
 *       so drawing a GFX_PIXEL_OFF triangle on top of already-OFF pixels
 *       will not fill correctly - this is intended for the normal case of
 *       filling with GFX_PIXEL_ON onto a cleared area.
 */
OLED_Status_t GFX_FillTriangle(SSD1306_t *oled, x_position_t x0, y_position_t y0, x_position_t x1, y_position_t y1, x_position_t x2, y_position_t y2 ,GFXPixel_t state);
/**
 * @brief   Draws an unfilled circle outline using the midpoint circle algorithm.
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       X coordinate of the circle's center.
 * @param[in]     y       Y coordinate of the circle's center.
 * @param[in]     radius  Circle radius in pixels.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                  Circle drawn successfully.
 * @return OLED_ERROR_BUFFER_OVERFLOW Circle extends past the display edge, or
 *                                    radius is larger than x or y (would
 *                                    extend past 0).
 */
OLED_Status_t GFX_DrawCircle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t radius, GFXPixel_t state);
/**
 * @brief   Draws a filled (solid) circle.
 *
 *          Uses the same midpoint algorithm as GFX_DrawCircle() but connects
 *          symmetric point pairs with horizontal spans instead of plotting
 *          single pixels.
 *
 * @param[in,out] oled    Pointer to the SSD1306 device structure.
 * @param[in]     x       X coordinate of the circle's center.
 * @param[in]     y       Y coordinate of the circle's center.
 * @param[in]     radius  Circle radius in pixels.
 * @param[in]     state   GFX_PIXEL_ON or GFX_PIXEL_OFF.
 *
 * @return OLED_OKAY                  Circle drawn successfully.
 * @return OLED_ERROR_BUFFER_OVERFLOW Circle extends past the display edge, or
 *                                    radius is larger than x or y (would
 *                                    extend past 0).
 */
OLED_Status_t GFX_FillCircle(SSD1306_t *oled, x_position_t x, y_position_t y, uint8_t radius, GFXPixel_t state);

//declare API to draw bitmap


/**
 * @brief Draw a monochrome bitmap onto the OLED frame buffer.
 *
 * Draws a 1-bit-per-pixel bitmap at the specified top-left position.
 * The bitmap is stored in row-major order with pixels encoded MSB-first.
 *
 * @param[in,out] oled      Pointer to the SSD1306 OLED device structure.
 * @param[in]     x         X-coordinate of the bitmap's top-left corner.
 * @param[in]     y         Y-coordinate of the bitmap's top-left corner.
 * @param[in]     bitmap    Pointer to the bitmap data.
 * @param[in]     width     Width of the bitmap in pixels.
 * @param[in]     height    Height of the bitmap in pixels.
 * @param[in]     state     Currently unused by the implementation - bitmap
 *                          pixels are drawn ON/OFF directly from the source
 *                          data's own bits, not remapped through this value.
 *
 * @return OLED_OKAY                  Bitmap was drawn successfully.
 * @return OLED_ERROR_BUFFER_OVERFLOW Bitmap extends outside the display boundaries.
 *
 * @note Bitmap format:
 *       - 1 bit per pixel (1 BPP)
 *       - 1 = pixel ON, 0 = pixel OFF
 *       - Row-major order (top-to-bottom, left-to-right)
 *       - MSB-first within each byte
 *       - Each row is padded to a whole number of bytes
 *         (bytes_per_row = (width + 7) / 8)
 */
OLED_Status_t GFX_DrawBitmap(SSD1306_t *oled,x_position_t x,y_position_t y,const uint8_t *bitmap,uint8_t width,uint8_t height,GFXPixel_t state);

//declare APIs to draw character, strings and numbers

/**
 * @brief   Draws a single character using the built-in 5x7 font.
 *
 *          Writes 5 bytes directly into one page of the framebuffer -
 *          the character does not need to be page-aligned in X, but it
 *          is confined to a single page in Y (8-pixel-tall row).
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     ch    Character to draw (ASCII 32-127).
 * @param[in]     col   Starting column, 0 to (SSD1306_WIDTH - 5).
 * @param[in]     page  Page (8-pixel-tall row), 0 to (SSD1306_PAGES - 1).
 *
 * @return OLED_OKAY                     Character drawn successfully.
 * @return OLED_ERROR_INVALID_CHARACTER  ch is outside the supported ASCII range.
 * @return OLED_ERROR_INVALID_COLUMN     col would place the character past the display edge.
 * @return OLED_ERROR_INVALID_PAGE       page is outside the valid range.
 */
OLED_Status_t GFX_DrawChar(SSD1306_t *oled, char ch, column col, page_number page);


/**
 * @brief   Draws a null-terminated string using the built-in 5x7 font.
 *
 *          Characters are spaced 7 columns apart (5 pixels of glyph + 2
 *          pixels of spacing). When a character would overflow the
 *          display width, printing continues from column 0 of the next
 *          page automatically.
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     str   Pointer to the null-terminated string to draw.
 * @param[in]     col   Starting column, 0 to (SSD1306_WIDTH - 5).
 * @param[in]     page  Starting page, 0 to (SSD1306_PAGES - 1).
 *
 * @return OLED_OKAY                     String drawn successfully.
 * @return OLED_ERROR_INVALID_COLUMN     Starting col is invalid.
 * @return OLED_ERROR_INVALID_PAGE       Starting page, or the page reached
 *                                       after wrapping, is out of range.
 * @return OLED_ERROR_INVALID_CHARACTER  str contains a character outside
 *                                       the supported ASCII range.
 */
OLED_Status_t GFX_DrawString(SSD1306_t *oled, char *str, column col, page_number page);

/**
 * @brief   Draws a signed integer as text, using the built-in 5x7 font.
 *
 *          Converts @p num to a decimal string internally and renders it
 *          via GFX_DrawString().
 *
 * @param[in,out] oled  Pointer to the SSD1306 device structure.
 * @param[in]     num   Integer value to display.
 * @param[in]     col   Starting column, 0 to (SSD1306_WIDTH - 5).
 * @param[in]     page  Starting page, 0 to (SSD1306_PAGES - 1).
 *
 * @return Status code returned by GFX_DrawString().
 */
OLED_Status_t GFX_DrawInt(SSD1306_t *oled, int num, column col, page_number page);

/**
 * @brief   Draws a floating-point value as text, using the built-in 5x7 font.
 *
 *          Converts @p num to a decimal string with @p decimals digits
 *          after the decimal point, then renders it via GFX_DrawString().
 *
 * @param[in,out] oled      Pointer to the SSD1306 device structure.
 * @param[in]     num       Floating-point value to display.
 * @param[in]     decimals  Number of digits to show after the decimal point.
 * @param[in]     col       Starting column, 0 to (SSD1306_WIDTH - 5).
 * @param[in]     page      Starting page, 0 to (SSD1306_PAGES - 1).
 *
 * @return Status code returned by GFX_DrawString().
 */
OLED_Status_t GFX_DrawFloat(SSD1306_t *oled, float num,uint8_t decimals, column col, page_number page);

#endif /* SSD1306_DISPLAY_INC_SSD1306_GRAPHICS_H_ */
