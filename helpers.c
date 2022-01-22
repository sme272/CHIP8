#include <time.h>
#include "helpers.h"
#include "instructions.h"

void initialize(CHIP8* chip8) {
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
}

void delay(uint8_t t) {
	clock_t start_time = clock();

	while (clock() < start_time + t) {
		;
	}
}

void execute_opcode(CHIP8* chip8) {
	if (chip8->opcode == 0x00E0) {
		cls(&chip8);
	}
	else if (chip8->opcode == 0x00EE) {
		ret(&chip8);
	}
	else if (0x1000 & chip8->opcode) {
		jp(&chip8);
	}
	else if (0x2000 & chip8->opcode) {
		call(&chip8);
	}
	else if (0x3000 & chip8->opcode) {
		sevxx(&chip8);
	}
	else if (0x4000 & chip8->opcode) {
		snevxx(&chip8);
	}
	else if (0x5000 & chip8->opcode) {
		sevxvy(&chip8);
	}
	else if (0x6000 & chip8->opcode) {
		ldvxx(&chip8);
	}
	else if (0x7000 & chip8->opcode) {
		addvxx(&chip8);
	}
	else if (0x8000 & chip8->opcode) {
		ldvxvy(&chip8);
	}
	else if (0x8001 & chip8->opcode) {
		orvxvy(&chip8);
	}
	else if (0x8002 & chip8->opcode) {
		andvxvy(&chip8);
	}
	else if (0x8003 & chip8->opcode) {
		xorvxvy(&chip8);
	}
	else if (0x8004 & chip8->opcode) {
		addvxvy(&chip8);
	} 
	else if (0x8005 & chip8->opcode) {
		subvxvy(&chip8);
	}
	else if (0x8006 & chip8->opcode) {
		shrvx(&chip8);
	}
	else if (0x8007 & chip8->opcode) {
		subn(&chip8);
	}
	else if (0x800e & chip8->opcode) {
		shlvx(&chip8);
	}
	else if (0x9000 & chip8->opcode) {
		snevxvy(&chip8);
	}
	else if (0xa000 & chip8->opcode) {
		ldi(&chip8);
	}
	else if (0xb000 & chip8->opcode) {
		jpvaddr(&chip8);
	}
	else if (0xc000 & chip8->opcode) {
		rndvxx(&chip8);
	}
	else if (0xd000 & chip8->opcode) {
		drwxy(&chip8);
	}
	else if (0xe09e & chip8->opcode) {
		skpvx(&chip8);
	}
	else if (0xe0a1 & chip8->opcode) {
		skpnvx(&chip8);
	}
	else if (0xf007 & chip8->opcode) {
		ldvxdt(&chip8);
	}
}
