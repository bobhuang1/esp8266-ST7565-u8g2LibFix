# The two-line fix, re-applied by hand

This repo patches u8g2's ST7565 driver (`u8x8_d_st7565.c`) for "Mini 12864"
panels. Re-apply after every u8g2 library update (an update overwrites the
installed file). A line-exact unified diff is not possible because the patched
file predates the oldest published u8g2 tag, but the two edits are tiny:

## 1. Display start line in the 64128N init sequence

In `u8x8_d_st7565_64128n_init_seq`, inside the non-`BIGBLUE12864` (Mini) code
path, find the display-start-line command of the second init sequence and
change it:

    U8X8_C(0x040),   /* set display start line to 0 */     <- stock
    U8X8_C(0x060),   /* Display start line for Displaytech 64128N */  <- Mini fix

There are multiple `U8X8_C(0x040)` lines in the file; the Mini one sits in the
init sequence guarded by `#ifndef BIGBLUE12864` / the `#else` branch of the
function (in the stock driver the function ends with the Mini branch). If in
doubt, diff this repo's `u8x8_d_st7565.c` against your installed library file
to see exactly which lines differ.

## 2. x_offset for the Mini panel

In `u8x8_st7565_64128n_display_info`, the Mini variant's `default_x_offset`
changes from `4` to `3`:

    /* default_x_offset = */ 4,   <- "New Big Blue 12864" variant: keep
    /* default_x_offset = */ 3,   <- Mini variant: this is the fix

The display_info struct with the `3` is the one selected for non-`BIGBLUE12864`
builds.

## Quick check

After re-applying, `diff` this repo's file against the freshly installed
library copy: the only differences must be the two lines above plus the
explanatory comment block at the top of this repo's file.
