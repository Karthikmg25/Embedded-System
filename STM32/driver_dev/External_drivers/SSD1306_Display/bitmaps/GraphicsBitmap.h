/*
 * GraphicsBitmap.h
 *
 *  Created on: Sep 5, 2026
 *      Author: hp
 */

#ifndef SSD1306_DISPLAY_BITMAPS_GRAPHICSBITMAP_H_
#define SSD1306_DISPLAY_BITMAPS_GRAPHICSBITMAP_H_

#include <stdint.h>
//declare bitmaps

extern const uint8_t heart_bitmap_32[];      // size: 32x32
extern const uint8_t heart_bitmap_8[];         // size  08X08
extern const uint8_t cat_bitmap_32[128];  //  size  32x32
extern const uint8_t cat_bitmap_60[480];  // 60x60
extern const uint8_t bee_bitmap_120x60[900];     //120x60
extern const uint8_t clock_bitmap_60x60[480];//60x60
extern const uint8_t clock_bitmap_30x30[120];//30x30
extern const uint8_t portrait_bitmap_64x64[512];
extern const uint8_t portrait2_bitmap_120x60[900];
extern const uint8_t portrait3_bitmap_100x60[780];
extern const uint8_t boy_bitmap_60x60[480];
extern const uint8_t walter_64x64[];
extern const uint8_t gus_bitmap_120x60[];
extern const uint8_t gus_bitmap_120x60_V2[];
extern const uint8_t gus_bitmap_contratst2_4[];

//      ball animation frames            /
/****************************************/
#define NUMBER_OF_FRAMES 10


extern const uint8_t ballFrame0[];
extern const uint8_t ballFrame1[];
extern const uint8_t ballFrame2[];
extern const uint8_t ballFrame3[];
extern const uint8_t ballFrame4[];
extern const uint8_t ballFrame5[];
extern const uint8_t ballFrame6[];
extern const uint8_t ballFrame7[];
extern const uint8_t ballFrame8[];
extern const uint8_t ballFrame9[];

//create an array of pointers to store frames
extern const uint8_t* ballFrames[];















#endif /* SSD1306_DISPLAY_BITMAPS_GRAPHICSBITMAP_H_ */
