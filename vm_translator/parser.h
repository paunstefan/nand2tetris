

#pragma once

enum command_t {
	C_NULL,
	C_ARITHMETIC, 
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};

/**
	Strips a line of comments.

	@param line: the line string which will be edited in place
*/
void strip_comments(char **line);

/**
	Splits a string into an array of words.

	@param line: the line to split
	@param tokens: the array containing the words
	@return the number of words written
*/
int tokenize(char *line, char ***tokens);

/**
	Checks the command type.

	@param tokens: array of strings containing the vm instruction
	@return the command type
*/
enum command_t get_command_type(char **tokens);

/**
	Checks if a label is valid.

	@param label: the label
	@return 1 if it is valid, else 0
*/
int check_label(char *label);

/**
	Returns the first index of a character in a string.

	@param line: the string
	@param ch: the character
	@return the index
*/
int index_of(char* line, char ch);
