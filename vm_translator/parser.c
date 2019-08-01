#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "parser.h"

/**
	Strips a line of comments.

	@param line: the line string which will be edited in place
*/
void strip_comments(char **line){
	char *ln = (char*)malloc(sizeof(char) * 128);
	memset(ln, 0, sizeof(char) * 128);

	for(int i = 0, j = 0; (*line)[i] != '\0'; i++){
		if((*line)[i] == '\n' || (*line)[i] == '\r' ||((*line)[i] == '/' && (*line)[i+1] == '/')){
			break;
		}
		if((*line)[i] == '\t'){
			continue;
		}

		ln[j] = (*line)[i];
		j++;
	}
	char *temp = *line;
	(*line) = ln;
	free(temp);

}

/**
	Checks if a label is valid.

	@param label: the label
	@return 1 if it is valid, else 0
*/
int check_label(char *label){

	if(isdigit(label[0])){
		return 0;
	}

	int len = strlen(label);
	for(int i = 0; i < len; i++){
		if(!isalnum(label[i]) && label[i] != '_' && label[i] != '.' && label[i] != ':'){
			printf("%c\n", label[i]);
			return 0;
		}
	}
	return 1;
}

/**
	Splits a string into an array of words.

	@param line: the line to split
	@param tokens: the array containing the words
	@return the number of words written
*/
int tokenize(char *line, char ***tokens){
	int counter = 0;
	char* token;

	while((token = strtok_r(line, " ", &line))){
		*tokens = (char**)realloc(*tokens, sizeof(char*) * (counter+1));
		(*tokens)[counter] = token;
		counter++;
	}

	return counter;
}

/**
	Returns the first index of a character in a string.

	@param line: the string
	@param ch: the character
	@return the index
*/
int index_of(char* line, char ch){
	for(int i = 0; line[i] != '\0'; i++){
		if(line[i] == ch){
			return i;
		}
	}
	return -1;
}

/**
	Checks the command type.

	@param tokens: array of strings containing the vm instruction
	@return the command type
*/
enum command_t get_command_type(char **tokens){
	if(strcmp(tokens[0], "add") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "sub") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "neg") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "eq") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "gt") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "lt") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "and") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "or") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "not") == 0){
		return C_ARITHMETIC;
	}
	else if(strcmp(tokens[0], "push") == 0){
		return C_PUSH;
	}
	else if(strcmp(tokens[0], "pop") == 0){
		return C_POP;
	}
	else if(strcmp(tokens[0], "call") == 0){
		return C_CALL;
	}
	else if(strcmp(tokens[0], "function") == 0){
		return C_FUNCTION;
	}
	else if(strcmp(tokens[0], "return") == 0){
		return C_RETURN;
	}
	else if(strcmp(tokens[0], "label") == 0){
		return C_LABEL;
	}
	else if(strcmp(tokens[0], "goto") == 0){
		return C_GOTO;
	}
	else if(strcmp(tokens[0], "if-goto") == 0){
		return C_IF;
	}
	else{
		return C_NULL;
	}
}