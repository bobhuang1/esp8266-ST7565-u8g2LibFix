# esp8266-ST7565-u8g2LibFix

A small local patch to [u8g2](https://github.com/olikraus/u8g2)'s ST7565
display driver (`u8x8_d_st7565.c`), fixing the display geometry for a
**"Mini 12864"** ST7565-based 128x64 LCD module that doesn't quite match the
stock driver's Displaytech 64128N timing.

## The problem

u8g2's `u8x8_d_st7565_64128n` driver already supports two board variants via
the `BIGBLUE12864` compile-time flag ("New Big Blue 12864" vs. the smaller
"Mini" board), but the values baked in for the Mini variant produce a shifted
or garbled display on at least one common "Mini 12864" module in the wild.

## The fix

Two values in `u8x8_d_st7565.c`, only in the non-`BIGBLUE12864` (Mini) code path:

| What | Stock u8g2 | This patch |
|---|---|---|
| Display start line command (`u8x8_d_st7565_64128n_init_seq`) | `0x040` | `0x060` |
| `default_x_offset` (`u8x8_st7565_64128n_display_info`) | `4` | `3` |

Everything else in the file is unmodified upstream u8g2 source, under u8g2's
own BSD-2-Clause license (see the header inside the file).

## How to use it

1. Locate your installed u8g2 library's clib folder, typically:
   `Documents/Arduino/libraries/U8g2/src/clib/u8x8_d_st7565.c`
2. Replace it with this repo's `u8x8_d_st7565.c`.
3. In your sketch, construct the display using the 64128N constructor (e.g.
   `U8G2_ST7565_64128N_F_4W_HW_SPI` or the software-SPI equivalent), **without**
   defining `BIGBLUE12864` at the top of this file, so the Mini-board code path
   (with the fix) is used.
4. If you're on the "New Big Blue 12864" board instead, uncomment
   `#define BIGBLUE12864` at the top of the file to use the original,
   unmodified stock values.

Re-apply this patch any time you update the u8g2 library, since a library
update will overwrite it with the stock file.
