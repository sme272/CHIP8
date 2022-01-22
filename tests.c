#include <stdint.h>
#include <stdlib.h>
#include <Unity/unity.h>
#include "helpers.h"
#include "instructions.h"

CHIP8 chip8;

void setUp(void) {
    initialize(&chip8);
}

void tearDown(void) {
    // clean stuff up here
}

void test_getOpcode(void) {
    chip8.ram[0x200] = 0x12;
    chip8.ram[0x201] = 0x34;
    get_opcode(&chip8);
    TEST_ASSERT(chip8.opcode == 0x1234);
    TEST_ASSERT(chip8.pc == 0x202);
}

void test_jp(void) {
    chip8.opcode = 0x1234;
    jp(&chip8);
    TEST_ASSERT(chip8.pc == 0x0234);
}

void test_ret(void) {
    chip8.sp = 1;
    chip8.stack[1] = 0x303;
    ret(&chip8);
    TEST_ASSERT(chip8.sp == 0);
    TEST_ASSERT(chip8.pc == 0x303);
}

void test_call(void){
    chip8.pc = 0x200;
    chip8.opcode = 0x2404;
    chip8.sp = 3;
    call(&chip8);
    TEST_ASSERT(chip8.sp == 4);
    TEST_ASSERT(chip8.stack[4] == 0x201);
    TEST_ASSERT(chip8.pc == 0x404);
}

void test_skipEqualVxByte(void) {
    chip8.opcode = 0x3245;
    chip8.V[2] = 0x45;
    sevxx(&chip8);
    TEST_ASSERT(chip8.pc == 0x202);

    chip8.opcode = 0x3345;
    chip8.V[3] = 0x00;
    chip8.pc = 0x200;
    sevxx(&chip8);
    TEST_ASSERT(chip8.pc == 0x200);
}

void test_skipNotEqualVxByte(void) {
    chip8.opcode = 0x4245;
    chip8.V[2] = 0x45;
    chip8.pc = 0x200;
    snevxx(&chip8);
    TEST_ASSERT(chip8.pc == 0x200);

    chip8.opcode = 0x4445;
    chip8.V[4] = 0x12;
    chip8.pc = 0x200;
    snevxx(&chip8);
    TEST_ASSERT(chip8.pc == 0x202);
}

void test_skipEqualVxVy(void) {
    chip8.opcode = 0x5120;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x12;
    chip8.pc = 0x200;
    sevxvy(&chip8);
    TEST_ASSERT(chip8.pc == 0x202);

    chip8.opcode= 0x5340;
    chip8.V[3] = 0x12;
    chip8.V[4] = 0x34;
    chip8.pc = 0x200;
    sevxvy(&chip8);
    TEST_ASSERT(chip8.pc == 0x200);
}

void test_loadVxByte(void) {
    chip8.opcode = 0x6245;
    ldvxx(&chip8);
    TEST_ASSERT(chip8.V[2] == 0x45);
}

void test_addVxByte(void) {
    chip8.opcode = 0x7123;
    chip8.V[1] = 0x1;
    addvxx(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x24);
}

void test_loadVxVy(void) {
    chip8.opcode = 0x8120;
    chip8.V[1] = 0x23;
    chip8.V[2] = 0x34;
    ldvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == chip8.V[2]);
}

void test_orVxVy(void) {
    chip8.opcode = 0x8121;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    orvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x36);
}

void test_andVxVy(void) {
    chip8.opcode = 0x8122;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    andvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x10);
}

void test_xorVxVy(void) {
    chip8.opcode = 0x8123;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    xorvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x26);
}

void test_addVxVy(void) {
    chip8.opcode = 0x8124;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    chip8.V[15] = 1;
    addvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x46);
    TEST_ASSERT(chip8.V[0xf] == 0);

    chip8.opcode = 0x8124;
    chip8.V[1] = 0xff;
    chip8.V[2] = 0xff;
    chip8.V[15] = 0;
    addvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0xfe);
    TEST_ASSERT(chip8.V[15] == 1);
}

void test_subVxVy(void) {
    chip8.opcode = 0x8125;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    chip8.V[15] = 1;
    subvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0xde);
    TEST_ASSERT(chip8.V[15] == 0x0);

    chip8.opcode = 0x8125;
    chip8.V[1] = 0x34;
    chip8.V[2] = 0x12;
    chip8.V[15] = 0;
    subvxvy(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x22);
    TEST_ASSERT(chip8.V[15] == 0x1);
}

void tesT_shiftRightVx(void) {
    chip8.opcode = 0x8126;
    chip8.V[1] = 0x12;
    chip8.V[15] = 1;
    shrvx(&chip8);
    TEST_ASSERT(chip8.V[15] == 0);
    TEST_ASSERT(chip8.V[1] == 0x9);

    chip8.opcode = 0x8126;
    chip8.V[1] = 0x23;
    chip8.V[15] = 0;
    shrvx(&chip8);
    TEST_ASSERT(chip8.V[15] == 1);
    TEST_ASSERT(chip8.V[1] == 0x11);
}

void test_subNotBorrow(void) {
    chip8.opcode = 0x8127;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x34;
    chip8.V[15] = 0;
    subn(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x22);
    TEST_ASSERT(chip8.V[15] == 0x1);

    chip8.opcode = 0x8347;
    chip8.V[3] = 0x34;
    chip8.V[4] = 0x12;
    chip8.V[15] = 1;
    subn(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x22);
    TEST_ASSERT(chip8.V[15] == 0x0);
}

void test_shiftLeftVx(void) {
    chip8.opcode = 0x8128;
    chip8.V[1] = 0x12;
    chip8.V[15] = 1;
    shlvx(&chip8);
    TEST_ASSERT(chip8.V[15] == 0);
    TEST_ASSERT(chip8.V[1] == 0x24);

    chip8.opcode = 0x8128;
    chip8.V[1] = 0xf0;
    shlvx(&chip8);
    TEST_ASSERT(chip8.V[15] == 1);
    TEST_ASSERT(chip8.V[1] == 0xe0);
}

void test_skipNotEqualVxVy(void) {
    chip8.opcode = 0x9120;
    chip8.V[1] = 0x12;
    chip8.V[2] = 0x12;
    chip8.pc = 0x200;
    snevxvy(&chip8);
    TEST_ASSERT(chip8.pc == 0x200);

    chip8.opcode = 0x9340;
    chip8.V[3] = 0x12;
    chip8.V[4] = 0x34;
    chip8.pc = 0x200;
    snevxvy(&chip8);
    TEST_ASSERT(chip8.pc == 0x202);
}

void test_loadI(void) {
    chip8.opcode = 0x9123;
    chip8.I = 0;
    ldi(&chip8);
    TEST_ASSERT(chip8.I == 0x123);
}

void test_jumpAddrV(void) {
    chip8.opcode = 0xb223;
    chip8.V[0] = 0x12;
    chip8.pc = 0x200;
    jpvaddr(&chip8);
    TEST_ASSERT(chip8.pc == 0x235);
}

void test_randAddByte(void) {
    chip8.opcode = 0xc123;
    srand(1); // Seed rand with a constant seed so rand returns a known value: 41
    rndvxx(&chip8);
    TEST_ASSERT(chip8.V[1] == 0x4c);
}

void test_drawxy(void) {
    // test if the appropriate bits are written
    chip8.opcode = 0xd124;
    chip8.I = 0x200;
    chip8.ram[0x200] = 0xea;
    chip8.ram[0x201] = 0xac;
    chip8.ram[0x202] = 0xaa;
    chip8.ram[0x203] = 0xea;
    drwxy(&chip8);
    TEST_ASSERT(chip8.gfx[2] == 0x7500000000000000);
    TEST_ASSERT(chip8.gfx[3] == 0x5600000000000000);
    TEST_ASSERT(chip8.gfx[4] == 0x5500000000000000);
    TEST_ASSERT(chip8.gfx[5] == 0x7500000000000000);
    TEST_ASSERT(chip8.V[15] == 0);

    // test collisions
    chip8.opcode = 0xd724;
    drwxy(&chip8);
    TEST_ASSERT(chip8.gfx[2] == 0x74d4000000000000);
    TEST_ASSERT(chip8.gfx[3] == 0x5758000000000000);
    TEST_ASSERT(chip8.gfx[4] == 0x5454000000000000);
    TEST_ASSERT(chip8.gfx[5] == 0x74d4000000000000);
    TEST_ASSERT(chip8.V[15] == 1);

}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_getOpcode);
    RUN_TEST(test_jp);
    RUN_TEST(test_ret);
    RUN_TEST(test_call);
    RUN_TEST(test_skipEqualVxByte);
    RUN_TEST(test_skipNotEqualVxByte);
    RUN_TEST(test_skipEqualVxVy);
    RUN_TEST(test_loadVxByte);
    RUN_TEST(test_addVxByte);
    RUN_TEST(test_loadVxByte);
    RUN_TEST(test_orVxVy);
    RUN_TEST(test_andVxVy);
    RUN_TEST(test_xorVxVy);
    RUN_TEST(test_addVxVy);
    RUN_TEST(test_subVxVy);
    RUN_TEST(tesT_shiftRightVx);
    RUN_TEST(test_subNotBorrow);
    RUN_TEST(test_shiftLeftVx);
    RUN_TEST(test_skipNotEqualVxVy);
    RUN_TEST(test_loadI);
    RUN_TEST(test_jumpAddrV);
    RUN_TEST(test_randAddByte);
    RUN_TEST(test_drawxy);
    return UNITY_END();
}