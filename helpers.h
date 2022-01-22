#pragma once

#include "CHIP8.h"

void initialize(CHIP8*);

void execute_opcode(CHIP8*);

void delay(uint8_t);