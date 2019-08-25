#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


/**
	Transforms an integer into its string binary representation.

	@param value: integer value
	@return the string representation
*/
char* number2string(uint16_t value){
	char* bin = (char*)malloc(sizeof(char) * 17);
	memset(bin, '0', 17);
	bin[16] = '\0';
	int index = 15;
	while(index){
		if((value & 1) == 1){
			bin[index] = '1';
		}

		value = value >> 1;
		index--;
	}

	return bin;

}

/**
	Creates an A instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void compile_A(uint16_t value, char** binary){
	char* str = number2string(value);

	memcpy((*binary)+1, str+1, 15);
	free(str);
}

/**
	Creates the JMP instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void get_jmp(uint16_t value, char** binary){
	char* str = number2string(value);

	memcpy((*binary)+13, str+13, 3);
	free(str);
}


/**
	Creates the DEST instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void get_dst(uint16_t value, char** binary){
	char* str = number2string(value);

	memcpy((*binary)+10, str+13, 3);
	free(str);
}


/**
	Creates the COMP instruction.

	@param value: parameter of the A instruction
	@param binary: string where to write the instruction
*/
void get_comp(uint16_t value, char** binary){
	char* str = number2string(value);

	memcpy((*binary)+3, str+9, 7);
	free(str);
}