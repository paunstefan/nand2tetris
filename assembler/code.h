#include <inttypes.h>

#pragma once


/**
	Creates an A instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void compile_A(uint16_t value, char** binary);

/**
	Creates the JMP instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
bool get_jmp(uint16_t value, char** binary);

/**
	Creates the DEST instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void get_dst(uint16_t value, char** binary);

/**
	Creates the COMP instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void get_comp(uint16_t value, char** binary);
