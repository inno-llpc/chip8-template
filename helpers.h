#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <stdint.h>


#define MS_IN_S	1000


void helper_initgfx(int width, int height);

void helper_deinitgfx();

void helper_set_draw_color(int r, int g, int b, int a);

void helper_draw_point(int x, int y);

void helper_clear_screen(int r, int g, int b, int a);

void helper_update_screen(void);

int helper_is_key_pressed(int key);

int helper_is_user_exit(void);

uint32_t helper_time(void);

void helper_sleep(uint32_t ms);

void helper_add_timer(uint32_t ms, void (*callback)(void));

void helper_remove_timer(void);

void helper_beep(void);

#endif

