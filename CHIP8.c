#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "CHIP8.h"
#include "instructions.h"
#include "helpers.h"


int main(int argc, char* argv[]) {

	CHIP8 chip8;

	init_cpu(&chip8);

	init_gfx();

	srand((unsigned)time(NULL));

	if (argc == 1) {
		load_program(&chip8, "test_opcode.ch8");
	}
	else if (argc == 2) {
		load_program(&chip8, argv[1]);
	}

	while (1){
		get_opcode(&chip8);
		execute_opcode(&chip8);
		delay(1);
	}

	return 0;
}
