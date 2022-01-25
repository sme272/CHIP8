#include <stdint.h>
#include "gfx.h"

SDL_Window* window;

SDL_Surface* surface;

SDL_Renderer* renderer;

void init_gfx(void) {
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1) {
		exit(-1);
	}

	atexit(close);

	window = NULL;
	window = SDL_CreateWindow("CHIP8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,640, 320, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		exit(-1);
	}
	else {
		surface = SDL_GetWindowSurface(window);
		renderer = SDL_CreateRenderer(window, 0, 0);
	}
}

void close(void) {
	SDL_DestroyRenderer(renderer);

    SDL_DestroyWindow(window);
    window = NULL;

    SDL_Quit();
}

void display(CHIP8* chip8) {
	SDL_Rect pixel;
	pixel.w = 10;
	pixel.h = 10;
	for (uint8_t y_offset = 0; y_offset < 32; y_offset++) {
		uint64_t row = chip8->gfx[y_offset];
		for (int8_t x_offset = 63; x_offset >= 0; x_offset--) {
			pixel.x = 10 * x_offset;
			pixel.y = 10 * y_offset;
			if (row & 0x1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			}
			SDL_RenderFillRect(renderer, &pixel);
			row >>= 1;
		}
	}
	SDL_RenderPresent(renderer);
}