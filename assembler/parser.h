#include <inttypes.h>

#pragma once

enum cmd_type{
	A_COMMAND,
	C_COMMAND,
	L_COMMAND
};

/**
	Returns the command type.

	@param line: the command
	@return the command
*/
enum cmd_type process(char** line);

/**
	Constructs the binary instruction from an input command line.

	@param line: the command
	@param binary: the string where to write the instruction
	@param type: the command type
	@param free_mem: the first free memory space
	@return true if it is a valid instruction, else false
*/
bool get_binary(char* line, char** binary, enum cmd_type type, uint16_t *free_mem);

/**
	Returns the first index of a character in a string.

	@param line: the string
	@param ch: the character
	@return the index
*/
int index_of(char* line, char ch);
