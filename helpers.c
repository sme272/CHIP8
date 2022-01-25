#include <time.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "helpers.h"
#include "instructions.h"

void init_cpu(CHIP8* chip8) {
	chip8->pc = 0x200;
	chip8->sp = 0;
	chip8->opcode = 0;

	chip8->I = 0;

	memset(chip8->ram, 0, 4096);
	memset(chip8->V, 0, 16);
	memset(chip8->stack, 0, 16);

	memset(chip8->gfx, 0, 32*8);

	chip8->sound_timer = 0;
	chip8->delay_timer = 0;

	load_charset(chip8);
}

void load_charset(CHIP8* chip8) {
	uint8_t charset[80] = {
		0xF0,0x90,0x90,0x90,0xF0,
		0x20,0x60,0x20,0x20,0x70,
		0xF0,0x10,0xF0,0x80,0xF0,
		0xF0,0x10,0xF0,0x10,0xF0,
		0x90,0x90,0xF0,0x10,0x10,
		0xF0,0x80,0xF0,0x10,0xF0,
		0xF0,0x80,0xF0,0x90,0xF0,
		0xF0,0x10,0x20,0x40,0x40,
		0xF0,0x90,0xF0,0x90,0xF0,
		0xF0,0x90,0xF0,0x10,0xF0,
		0xF0,0x90,0xF0,0x90,0x90,
		0xE0,0x90,0xE0,0x90,0xE0,
		0xF0,0x80,0x80,0x80,0xF0,
		0xE0,0x90,0x90,0x90,0xE0,
		0xF0,0x80,0xF0,0x80,0xF0,
		0xF0,0x80,0xF0,0x80,0x80
	};

	for (uint8_t i = 0; i < 80; i++) {
		chip8->ram[i] = charset[i];
	}
}

void load_program(CHIP8* chip8, char* file) {
	FILE* prg;
	prg = fopen(file, "rb");
	if (prg == NULL) {
		printf("%d", errno);
		exit(errno);
	}

	uint16_t ram_loc = 0x200;
	uint8_t buffer = 0;
	while (fread(&buffer, sizeof(uint8_t), 1, prg)) {
		chip8->ram[ram_loc] = buffer;
		ram_loc++;
	}
}

void delay(uint8_t t) {
	clock_t start_time = clock();

	while (clock() < start_time + t) {
		;
	}
}

void execute_opcode(CHIP8* chip8) {

	switch (chip8->opcode & 0xF000) {
		case 0x0000:
			switch (chip8->opcode & 0xFF) {
				case 0x00E0: cls(chip8); break;
				case 0x00EE: ret(chip8); break;
		} break;
		case 0x1000: jp(chip8); break;
		case 0x2000: call(chip8); break;
		case 0x3000: sevxx(chip8); break;
		case 0x4000: snevxx(chip8); break;
		case 0x5000: sevxvy(chip8); break;
		case 0x6000: ldvxx(chip8); break;
		case 0x7000: addvxx(chip8); break;
		case 0x8000:
			switch (chip8->opcode & 0xF) {
				case 0x0: ldvxvy(chip8); break;
				case 0x1: orvxvy(chip8); break;
				case 0x2: andvxvy(chip8); break;
				case 0x3: xorvxvy(chip8); break;
				case 0x4: addvxvy(chip8); break;
				case 0x5: subvxvy(chip8); break;
				case 0x6: shrvx(chip8); break;
				case 0x7: subn(chip8); break;
				case 0x8: shlvx(chip8); break;
			} break;
		case 0x9000: snevxvy(chip8); break;
		case 0xA000: ldi(chip8); break;
		case 0xB000: jpvaddr(chip8); break;
		case 0xC000: rndvxx(chip8); break;
		case 0xD000: drwxy(chip8); break;
		case 0xE000:
			switch (chip8->opcode & 0x00FF) {
				case 0x9E: skpvx(chip8); break;
				case 0xA1: skpnvx(chip8); break;
			} break;
		case 0xF000:
			switch (chip8->opcode & 0x00FF) {
			case 0x07: ldvxdt(chip8); break;
			case 0x0A: ldvxk(chip8); break;
			case 0x15: lddt(chip8); break;
			case 0x18: ldst(chip8); break;
			case 0x1E: addivx(chip8); break;
			case 0x29: ldfvx(chip8); break;
			case 0x33: ldbvx(chip8); break;
			case 0x55: stvx(chip8); break;
			case 0x65: ldvx(chip8); break;
			}break;
	}


}
