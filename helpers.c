#include <SDL2/SDL.h>

#include "helpers.h"


static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_TimerID timer;


void
helper_initgfx(int width, int height)
{
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Init(SDL_INIT_TIMER);
	SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);

	SDL_RenderPresent(renderer);
}

void
helper_deinitgfx()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
helper_set_draw_color(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
}

void
helper_draw_point(int x, int y)
{
	SDL_RenderDrawPoint(renderer, x, y);
}

void
helper_clear_screen(int r, int g, int b, int a)
{
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_RenderClear(renderer);
}

void
helper_update_screen(void)
{
	SDL_RenderPresent(renderer);
}

int
helper_is_key_pressed(int key)
{
	const uint8_t *state = SDL_GetKeyboardState(NULL);

#define K(c8k, sdlk)	[c8k] = SDL_SCANCODE_##sdlk,
	const int keys[0x10] = {
		K(0x1, 1) K(0x2, 2) K(0x3, 3) K(0xC, 4)
		K(0x4, Q) K(0x5, W) K(0x6, E) K(0xD, R)
		K(0x7, A) K(0x8, S) K(0x9, D) K(0xE, F)
		K(0xA, Z) K(0x0, X) K(0xB, C) K(0xF, V)
	};
#undef K

	if (key < 0x10)
		return state[keys[key]];
	return 0;

}

int
helper_is_user_exit(void)
{
	SDL_Event event;
	return SDL_PollEvent(&event) && event.type == SDL_QUIT;
}

uint32_t
helper_time(void)
{
	return SDL_GetTicks();
}

void
helper_sleep(uint32_t ms)
{
	SDL_Delay(ms);
}

static void (*cb)(void);

static uint32_t timer_callback(uint32_t ms, void *params)
{
	(void) params;
	cb();
	return ms;
}

void
helper_add_timer(uint32_t ms, void (*callback)(void))
{
	cb = callback;
	timer = SDL_AddTimer(ms, timer_callback, NULL);
}

void
helper_remove_timer(void)
{
	SDL_RemoveTimer(timer);
}

#ifdef WIN32
#include "windows.h"
void
helper_beep(void)
{
	MessageBeep(0);
}
#else
void
helper_beep(void)
{
	int _ = system("beep");
	(void) _;
}
#endif
