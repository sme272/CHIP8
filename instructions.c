#include <stdint.h>
#include <stdlib.h>
#include "instructions.h"

void get_opcode(CHIP8* chip8) {
	//Opcode is 2 bytes long, first byte located at pc, second immediately after.
	chip8->opcode = chip8->ram[chip8->pc] << 8 | chip8->ram[chip8->pc + 1];
	chip8->pc += 2;
}

void cls(CHIP8* chip8) {
	for (uint16_t i = 0; i < 64 * 32; i++) {
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
	chip8->stack[chip8->sp] = ++chip8->pc;
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
	uint8_t y_reg = (chip8->opcode > 4) & 0xf;
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