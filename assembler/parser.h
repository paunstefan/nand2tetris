#include <inttypes.h>

#ifndef PARSER_H
#define PARSER_H

enum cmd_type{
	A_COMMAND,
	C_COMMAND,
	L_COMMAND
};


enum cmd_type process(char** line);
bool get_binary(char* line, char** binary, enum cmd_type type, uint16_t *free_mem);
int index_of(char* line, char ch);

#endif