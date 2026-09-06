# SSD1306 OLED Driver + Graphics Layer

A bare-metal driver for the SSD1306 128x64 monochrome OLED display, built
from register-level I2C communication up to a full graphics API. No vendor
HAL shortcuts, no Arduino libraries — direct register access, currently
targeting STM32F401.

## Architecture

The driver is split into two layers:

```
┌─────────────────────────────────────────────┐
│           Your application (main.c)          │
└───────────────────────┬───────────────────────┘
                         │
┌───────────────────────▼───────────────────────┐
│   SSD1306_Graphics.c/h   — graphics layer     │
│   pixels, lines, shapes, bitmaps, text        │
│   (reads/writes SSD1306_t.frame_buffer only)  │
└───────────────────────┬───────────────────────┘
                         │
┌───────────────────────▼───────────────────────┐
│   SSD1306.c/h   — controller driver           │
│   init, contrast, on/off/invert,              │
│   framebuffer → display RAM transfer over I2C │
└───────────────────────┬───────────────────────┘
                         │
┌───────────────────────▼───────────────────────┐
│                  I2C driver                   │
│   e.g. stm32f401re_i2c_driver.h               │
└───────────────────────────────────────────────┘
```

**Why this split:** the graphics layer never touches I2C or the display
directly — it only reads and writes the `frame_buffer` array inside the
`SSD1306_t` struct. This means every `GFX_*` drawing call is just RAM
manipulation and is effectively free of hardware timing concerns; nothing
is sent to the physical display until `SSD1306_Update()` is called
explicitly. That separation is also what made it straightforward to build
and test frame-based animation on top (see below) without touching either
driver layer.

## Files

| File | Responsibility |
|---|---|
| `SSD1306.h` / `SSD1306.c` | Controller driver: init sequence, orientation, contrast, display on/off/invert, `SSD1306_Update()` (framebuffer → display RAM over I2C). |
| `SSD1306_Graphics.h` / `SSD1306_Graphics.c` | Graphics layer: pixels, lines, rectangles, triangles, circles (outline + filled), 1bpp bitmaps, and text (built-in 5x7 font). |
| `GraphicsBitmap.h` / `GraphicsBitmap.c` | Bitmap/frame-array storage for `GFX_DrawBitmap()`, including multi-frame animation sequences. |

## Usage

```c
SSD1306_t oled;
oled.I2C_Address  = OLED_ADDRESS;
oled.I2C_interface = I2C1;
oled.contrast      = 0xFF;
oled.orientation   = OLED_ORIENTATION_ROTATE180;

SSD1306_Init(&oled);

GFX_Clear(&oled);
GFX_DrawString(&oled, "Hello!", 0, 0);
GFX_DrawCircle(&oled, 64, 40, 15, GFX_PIXEL_ON);
SSD1306_Update(&oled);   // nothing appears on the physical display until this call
```

Every `GFX_*` and `SSD1306_ClearDisplay`/`SetContrast`/etc. call only
modifies the local `frame_buffer` — always finish a drawing sequence with
`SSD1306_Update(&oled)` to push it to the screen.

### Animation

`GFX_DrawBitmap()` draws one 1bpp bitmap per call, so animation is just:
clear → draw one frame → update → delay → repeat.

```c
while(1)
{
    for (uint8_t i = 0; i < NUMBER_OF_FRAMES; i++)
    {
        GFX_Clear(&oled);
        GFX_DrawBitmap(&oled, 0, 0, ballFrames[i], 120, 60, GFX_PIXEL_ON);
        SSD1306_Update(&oled);
        force_delay();
    }
}
```

`ballFrames[]` is a plain array of `const uint8_t*`, each pointing at one
frame's bitmap data (see GraphicsBitmap.h). Frame data is generated
offline (e.g. by converting a PNG through dithering into a packed 1bpp
byte array) rather than at runtime.

## Bitmap format (`GFX_DrawBitmap`)

- 1 bit per pixel — 1 = pixel ON, 0 = pixel OFF
- Row-major order (top-to-bottom, left-to-right)
- MSB-first within each byte
- Each row padded to a whole number of bytes:
  `bytes_per_row = (width + 7) / 8`

This matches the format produced by common bitmap-to-C-array tools
(image2cpp, LCD Assistant) when set to horizontal/row-major, MSB-first
output — no reformatting needed between conversion and `GFX_DrawBitmap()`.

> **Known limitation:** `GFX_DrawBitmap()`'s `state` parameter is currently
> unused — pixels are drawn ON/OFF directly from the bitmap's own bits,
> not remapped through `state`. Passing `GFX_PIXEL_OFF` will not invert
> the bitmap.

## Framebuffer layout

`frame_buffer` is `SSD1306_WIDTH * SSD1306_HEIGHT / 8` bytes (1024 bytes
for 128x64), organized as 8 pages of 128 bytes each. Within a page, each
byte represents one column across 8 vertically-stacked pixels (LSB =
top pixel of that page). `GFX_DrawPixel()` is the only place this layout
is computed directly — every other `GFX_*` function is built on top of
`GFX_DrawPixel()` / `GFX_DrawLine()`, so the byte/bit math never needs to
be repeated elsewhere.

## Status codes

All drawing/init functions that can fail return an `OLED_Status_t`:

| Code | Meaning |
|---|---|
| `OLED_OKAY` | Success |
| `OLED_ERROR_INVALID_COORDINATE` | x/y outside display bounds |
| `OLED_ERROR_BUFFER_OVERFLOW` | Shape/bitmap would extend past the display edge |
| `OLED_ERROR_INVALID_COLUMN` / `OLED_ERROR_INVALID_PAGE` | Text position out of range |
| `OLED_ERROR_INVALID_CHARACTER` | Character outside supported ASCII range (32-127) |
| `OLED_ERROR_INVALID_ORIENTATION` | Invalid value in `SSD1306_t.orientation` |

## Hardware

- Display: SSD1306-based 128x64 monochrome OLED, I2C interface (default address `0x3C`)
- Tested on: STM32F401RE (bare-metal, no HAL)
- I2C driver dependency: project-supplied (`stm32f401re_i2c_driver.h`), exposing `I2C_Reg_t`, `I2C_Transmit_Byte()`, `I2C_Transmit_Buffer()`

## Extending

- New drawing primitives should be added to `SSD1306_Graphics.c/h` and
  built on top of `GFX_DrawPixel()`/`GFX_DrawLine()` where possible, to
  keep framebuffer-layout knowledge in one place.
- New animation frame sets go in `GraphicsBitmap.c/h` as a bitmap array
  plus a `const uint8_t*` pointer table, following the pattern used for
  `ballFrames[]`.
