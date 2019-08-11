# Chip-8 emulator template

Here is template for chip-8 emulator.

## Requirements

* `libsdl2`
* `make`

## Helpers

There are sever helper functions for you:

* `void helper_initgfx(int w, int h)` -- inits library and opens window `w` by `h` pixels.
* `void helper_deinitgfx()` -- deinits library.
* `void helper_draw_point(int x, int y)` -- setx pixels at `x`,`y`
* `void helper_set_draw_color(int r, int g, int b, int a)` -- sets color for `helper_draw_point`.
* `void helper_clear_screen(int r, int g, int b, int a)` -- sets screen to one color.
* `void helper_clear_screen(int r, int g, int b, int a)` -- sets screen to one color.
* `void helper_update_screen()` -- update screen with all new changes.
* `int helper_is_key_pressed(int key)` -- takes number from 1 to 15 as argument, returns 1 if that key is pressed. Look for keyboard mappings in [helpers.h](helpers.h).
* `int helper_is_user_exit()` -- returns 1 if user tried to exit from window.
* `uint32_t helper_time()` -- returns number of milliseconds passed from begining of exection.
* `void helper_sleep(uint32_t ms)` -- sleeps for `ms` milliseconds.
* `void helper_add_timer(uint32_t ms, void (*callback)(void))` -- adds timer which will call `callback` every `ms` milliseconds.
* `void helper_beep(uint8_t enable)` -- starts beeping if 1 is passed, stops otherwise.

## Info

Some info about chip-8 available at our [wiki](https://wiki.innollp.club/ChipEightEmul) or in file [INFO.md](INFO.md).

Good luck!

