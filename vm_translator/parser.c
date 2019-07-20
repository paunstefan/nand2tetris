#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"


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
	else{
		return C_NULL;
	}
}