#pragma once
#include "CHIP8.h"


void get_opcode(CHIP8*);

void cls(CHIP8*);

void ret(CHIP8*);

void jp(CHIP8*);

void call(CHIP8*);

void sevxx(CHIP8*);

void snevxx(CHIP8*);

void sevxvy(CHIP8*);

void ldvxx(CHIP8*);

void addvxx(CHIP8*);

void ldvxvy(CHIP8*);

void orvxvy(CHIP8*);

void andvxvy(CHIP8*);

void xorvxvy(CHIP8*);

void addvxvy(CHIP8*);

void subvxvy(CHIP8*);

void shrvx(CHIP8*);

void subn(CHIP8*);

void shlvx(CHIP8*);

void snevxvy(CHIP8*);

void ldi(CHIP8*);

void jpvaddr(CHIP8*);

void rndvxx(CHIP8*);

void drwvxvy(CHIP8*);