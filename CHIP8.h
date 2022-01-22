#pragma once
#include <stdint.h>

typedef struct CHIP8 {
	uint16_t pc;
	uint16_t opcode;
	uint16_t I;
	uint8_t sp;

	uint8_t V[16];
	uint16_t stack[16];
	uint8_t ram[4096];

	uint64_t gfx[32];
	uint8_t key[16];

	uint8_t sound_timer;
	uint8_t delay_timer;
}CHIP8;
