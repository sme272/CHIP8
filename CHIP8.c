#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "CHIP8.h"
#include "instructions.h"
#include "helpers.h"


int main() {

	CHIP8 chip8;

	initialize(&chip8);

	srand((unsigned)time(NULL));

	while (1){
		get_opcode(&chip8);
		execute_opcode(&chip8);
	}

	return 0;
}
