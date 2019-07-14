#include <inttypes.h>

#ifndef CODE_H
#define CODE_H

void compile_A(uint16_t value, char** binary);
bool get_jmp(uint16_t value, char** binary);
void get_dst(uint16_t value, char** binary);
void get_comp(uint16_t value, char** binary);

#endif