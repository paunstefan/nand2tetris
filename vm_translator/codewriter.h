
#pragma once

/**
	Chooses which instruction to write and calls the needed function.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@param file: the name of the file containing the instruction
	@return the number of characters written
*/
int compile(char **tokens, char **instruction, char *file);