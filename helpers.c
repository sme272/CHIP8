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

	memset(chip8->gfx, 0, 32);

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
	if (chip8->opcode == 0x00E0) {
		cls(chip8);
	}
	else if (chip8->opcode == 0x00EE) {
		ret(chip8);
	}
	else if (0x1000 & chip8->opcode) {
		jp(chip8);
	}
	else if (0x2000 & chip8->opcode) {
		call(chip8);
	}
	else if (0x3000 & chip8->opcode) {
		sevxx(chip8);
	}
	else if (0x4000 & chip8->opcode) {
		snevxx(chip8);
	}
	else if (0x5000 & chip8->opcode) {
		sevxvy(chip8);
	}
	else if (0x6000 & chip8->opcode) {
		ldvxx(chip8);
	}
	else if (0x7000 & chip8->opcode) {
		addvxx(chip8);
	}
	else if (0x8000 & chip8->opcode) {
		ldvxvy(chip8);
	}
	else if (0x8001 & chip8->opcode) {
		orvxvy(chip8);
	}
	else if (0x8002 & chip8->opcode) {
		andvxvy(chip8);
	}
	else if (0x8003 & chip8->opcode) {
		xorvxvy(chip8);
	}
	else if (0x8004 & chip8->opcode) {
		addvxvy(chip8);
	} 
	else if (0x8005 & chip8->opcode) {
		subvxvy(chip8);
	}
	else if (0x8006 & chip8->opcode) {
		shrvx(chip8);
	}
	else if (0x8007 & chip8->opcode) {
		subn(chip8);
	}
	else if (0x800e & chip8->opcode) {
		shlvx(chip8);
	}
	else if (0x9000 & chip8->opcode) {
		snevxvy(chip8);
	}
	else if (0xa000 & chip8->opcode) {
		ldi(chip8);
	}
	else if (0xb000 & chip8->opcode) {
		jpvaddr(chip8);
	}
	else if (0xc000 & chip8->opcode) {
		rndvxx(chip8);
	}
	else if (0xd000 & chip8->opcode) {
		drwxy(chip8);
	}
	else if (0xe09e & chip8->opcode) {
		skpvx(chip8);
	}
	else if (0xe0a1 & chip8->opcode) {
		skpnvx(chip8);
	}
	else if (0xf007 & chip8->opcode) {
		ldvxdt(chip8);
	}
	else if (0xf00a & chip8->opcode) {
		ldvxk(chip8);
	}
	else if (0xf015 & chip8->opcode) {
		lddt(chip8);
	}
	else if (0xf018 & chip8->opcode) {
		ldst(chip8);
	}
	else if (0xf01e & chip8->opcode) {
		addivx(chip8);
	}
	else if (0xf029 & chip8->opcode) {
		ldfvx(chip8);
	}
	else if (0xf033 & chip8->opcode) {
		ldbvx(chip8);
	}
	else if (0xf055 & chip8->opcode) {
		stvx(chip8);
	}
	else if (0xf065 & chip8->opcode) {
		ldvx(chip8);
	}
}
