#pragma once

#include "CHIP8.h"

void init_cpu(CHIP8*);

void execute_opcode(CHIP8*);

void delay(uint8_t);

void load_charset(CHIP8*);

void load_program(CHIP8*, char*);