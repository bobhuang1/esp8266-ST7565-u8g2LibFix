# esp8266-ST7565-u8g2LibFix

Fixes the display geometry of a **"Mini 12864"** ST7565-based 128x64 LCD
module when driven by [u8g2](https://github.com/olikraus/u8g2)'s Displaytech
64128N driver (`U8G2_ST7565_64128N_...`). With stock u8g2 the picture on this
panel is shifted or garbled; the "New Big Blue 12864" panel works with stock
u8g2 as is.

## The fix

Two values differ from stock u8g2:

| What | Stock u8g2 | Mini 12864 |
|---|---|---|
| Display start line command (64128N init sequence) | `0x40` | `0x60` |
| Column offset (`default_x_offset`) | `4` | `3` |

## Recommended: apply it at runtime

No library patch is needed. Use the stock u8g2 library and set both values
right after `begin()`:

```cpp
display.begin();
display.getU8x8()->x_offset = 3;  // column offset for the Mini panel
display.sendF("c", 0x60);         // display start line
```

[`examples/MiniFix/MiniFix.ino`](examples/MiniFix/MiniFix.ino) is a complete
sketch. Checked against u8g2 2.36.5. Call it again after `setFlipMode()`,
which resets the column offset, and after any re-`begin()`.

This survives u8g2 updates, keeps upstream fixes, and lets one sketch choose
the panel at runtime.

## Legacy: the patched library file

`u8x8_d_st7565.c` is the older approach: a copy of u8g2's driver file with the
values above. It predates every published u8g2 tag, so **do not copy it over a
current u8g2 install**: current releases declare ST7565 variants this file
does not define (link errors), and it would revert upstream fixes to the other
drivers in the file.

The file adds two switches of its own; neither exists in upstream u8g2:

- `BIGBLUE12864` selects the stock values (for the Big Blue panel). Without it
  the file uses the Mini values.
- `INVERSE_DISPLAY` makes the `64128n` and `lm6059` drivers start in reverse
  video (`0xA7` instead of `0xA6`). The runtime equivalent is
  `display.sendF("c", 0xa7);`.

The patched file keeps u8g2's BSD-2-Clause license (see its header). The
GPL-3.0 `LICENSE` covers only this repository's own files (README, FIX.md,
the example).

### Installing the legacy file (old u8g2 only)

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
