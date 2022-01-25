#include <stdint.h>
#include <stdlib.h>
#include "instructions.h"
#include "helpers.h"
#include "gfx.h"

void get_opcode(CHIP8* chip8) {
	//Opcode is 2 bytes long, first byte located at pc, second immediately after.
	chip8->opcode = chip8->ram[chip8->pc] << 8 | chip8->ram[chip8->pc + 1];
	chip8->pc += 2;
}

void cls(CHIP8* chip8) {
	for (uint16_t i = 0; i < 32; i++) {
		chip8->gfx[i] = 0;
	}
}

void jp(CHIP8* chip8) {
	chip8->pc = chip8->opcode & 0x0fff;
}

void ret(CHIP8* chip8) {
	chip8->pc = chip8->stack[chip8->sp];
	chip8->sp--;
}

void call(CHIP8* chip8) {
	chip8->sp++;
	chip8->stack[chip8->sp] = chip8->pc;
	chip8->pc = chip8->opcode & 0x0FFF;
}

void sevxx(CHIP8* chip8) {
	uint8_t op_byte = chip8->opcode & 0xff;
	uint8_t op_register = (chip8->opcode >> 8) & 0xf;
	if (op_byte == chip8->V[op_register]) {
		chip8->pc+=2;
	}
}

void snevxx(CHIP8* chip8) {
	uint8_t op_byte = chip8->opcode & 0xff;
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;
	if (op_byte != chip8->V[op_reg]) {
		chip8->pc += 2;
	}
}

void sevxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	if (chip8->V[x_reg] == chip8->V[y_reg]) {
		chip8->pc += 2;
	}
}

void ldvxx(CHIP8* chip8) {
	uint8_t op_byte = chip8->opcode & 0xff;
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;
	chip8->V[op_reg] = op_byte;
}

void addvxx(CHIP8* chip8) {
	uint8_t op_byte = chip8->opcode & 0xff;
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;
	chip8->V[op_reg] += op_byte;
}

void ldvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	chip8->V[x_reg] = chip8->V[y_reg];
}

void orvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	chip8->V[x_reg] = chip8->V[x_reg] | chip8->V[y_reg];
}

void andvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	chip8->V[x_reg] = chip8->V[x_reg] & chip8->V[y_reg];
}

void xorvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	chip8->V[x_reg] = chip8->V[x_reg] ^ chip8->V[y_reg];
}

void addvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	
	if (chip8->V[x_reg] + chip8->V[y_reg] > 255) {
		chip8->V[15] = 1;
	}
	else {
		chip8->V[15] = 0;
	}
	chip8->V[x_reg] += chip8->V[y_reg];
}

void subvxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;

	if (chip8->V[x_reg] > chip8->V[y_reg]) {
		chip8->V[15] = 1;
	}
	else {
		chip8->V[15] = 0;
	}

	chip8->V[x_reg] -= chip8->V[y_reg];
}

void shrvx(CHIP8* chip8) {
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;

	chip8->V[15] = chip8->V[op_reg] & 0x1;
	chip8->V[op_reg] = chip8->V[op_reg] >> 1;
}

void subn(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;

	if (chip8->V[y_reg] > chip8->V[x_reg]) {
		chip8->V[15] = 1;
	}
	else {
		chip8->V[15] = 0;
	}

	chip8->V[x_reg] = chip8->V[y_reg] - chip8->V[x_reg];
}

void shlvx(CHIP8* chip8) {
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;

	chip8->V[15] = (chip8->V[op_reg] & 0xf0) >> 7;
	chip8->V[op_reg] = chip8->V[op_reg] << 1;
}

void snevxvy(CHIP8* chip8) {
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;
	if (chip8->V[x_reg] != chip8->V[y_reg]) {
		chip8->pc += 2;
	}
}

void ldi(CHIP8* chip8) {
	chip8->I = chip8->opcode & 0xfff;
}

void jpvaddr(CHIP8* chip8) {
	chip8->pc = chip8->V[0] + (chip8->opcode & 0xfff);
}

void rndvxx(CHIP8* chip8) {
	uint8_t op_byte = chip8->opcode & 0xff;
	uint8_t op_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t rnd = rand() % 256;
	chip8->V[op_reg] = op_byte + rnd;
}

void drwxy(CHIP8* chip8) {
	uint8_t n = chip8->opcode & 0xf;
	uint8_t x_reg = (chip8->opcode >> 8) & 0xf;
	uint8_t y_reg = (chip8->opcode >> 4) & 0xf;

	uint8_t x = chip8->V[x_reg];
	uint8_t y = chip8->V[y_reg];

	uint64_t row = 0;


	chip8->V[15] = 0;

	for (uint8_t i = 0; i < n; i++) {
		row = chip8->ram[chip8->I + i];
		if (x > 56) {
			row >>= 64 - x;
		}
		else {
			row <<= 64 - 8 - x;
		}
		if (chip8->gfx[y + i] & row) {
			chip8->V[15] = 1;
		}
		chip8->gfx[y + i] ^= row;
	}
	display(chip8);
}

void skpvx(CHIP8* chip8) {
	uint8_t key = 0x1 << (chip8->opcode >> 8 & 0xf);
	if (chip8->key & key) {
		chip8->pc += 2;
	}
}

void skpnvx(CHIP8* chip8) {
	uint8_t key = 0x1 << (chip8->opcode >> 8 & 0xf);
	if (!(chip8->key & key)) {
		chip8->pc += 2;
	}
}

void ldvxdt(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	chip8->V[x_reg] = chip8->delay_timer;
}

void ldvxk(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	while (!chip8->key) {
		delay(20);
	}

	uint8_t j = 1;
	for (uint8_t i = chip8->key; !(i&1); i >>= 1) {
		j <<= 1;
	}
	chip8->V[x_reg] = j;
}

void lddt(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	chip8->delay_timer = chip8->V[x_reg];
}

void ldst(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	chip8->sound_timer = chip8->V[x_reg];
}

void addivx(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	chip8->I += chip8->V[x_reg];
}

void ldfvx(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	chip8->I = 5 * chip8->V[x_reg];
}

void ldbvx(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;

	uint8_t value = chip8->V[x_reg];
	for (int8_t i = 2; i >= 0; i--) {
		chip8->ram[chip8->I + i] = value % 10;
		value /= 10;
	}
}

void stvx(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	for (uint8_t i = 0; i <= x_reg; i++) {
		chip8->ram[chip8->I + i] = chip8->V[i];
	}
}

void ldvx(CHIP8* chip8) {
	uint8_t x_reg = chip8->opcode >> 8 & 0xf;
	for (uint8_t i = 0; i <= x_reg; i++) {
		chip8->V[i] = chip8->ram[chip8->I + i];
	}
}