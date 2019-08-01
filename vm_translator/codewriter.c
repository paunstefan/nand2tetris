#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "parser.h"
#include "global.h"


#define POP_D "@SP\nM=M-1\nD=M\nA=D\nD=M"
#define PUSH_D "@SP\nA=M\nM=D\n@SP\nM=M+1"

/**
	Writes the arithmetic operations.

	@param op: the operation string
	@param instr: the string to which to write the instruction
	@return the number of characters written
*/
int write_arithmetic(char *op, char **instr){
	int wr = -1;

	// 1 op
	if(strcmp(op, "neg") == 0){
		char format[64] = "%s\nD=-D\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, PUSH_D);
	}
	else if(strcmp(op, "not") == 0){
		char format[64] = "%s\nD=!D\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, PUSH_D);
	}
	// 2 op
	else if(strcmp(op, "add") == 0){
		char format[64] = "%s\n@R13\nM=D\n%s\n@R13\nD=D+M\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, PUSH_D);
	}
	else if(strcmp(op, "sub") == 0){
		char format[64] = "%s\n@R13\nM=D\n%s\n@R13\nD=D-M\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, PUSH_D);
	}
	else if(strcmp(op, "and") == 0){
		char format[64] = "%s\n@R13\nM=D\n%s\n@R13\nD=D&M\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, PUSH_D);
	}
	else if(strcmp(op, "or") == 0){
		char format[64] = "%s\n@R13\nM=D\n%s\n@R13\nD=D|M\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, PUSH_D);
	}
	// comparison
	else if(strcmp(op, "eq") == 0){
		char format[100] = "%s\n@R13\nM=D\n%s\n@R13\nD=D-M\n@true%d\nD;JEQ\nD=0\n@end%d\n0;JMP\n(true%d)\nD=-1\n(end%d)\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, jmp_c, jmp_c, jmp_c, jmp_c, PUSH_D);
		jmp_c++;
	}
	else if(strcmp(op, "gt") == 0){
		char format[100] = "%s\n@R13\nM=D\n%s\n@R13\nD=D-M\n@true%d\nD;JGT\nD=0\n@end%d\n0;JMP\n(true%d)\nD=-1\n(end%d)\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, jmp_c, jmp_c, jmp_c, jmp_c, PUSH_D);
		jmp_c++;
	}
	else if(strcmp(op, "lt") == 0){
		char format[100] = "%s\n@R13\nM=D\n%s\n@R13\nD=D-M\n@true%d\nD;JLT\nD=0\n@end%d\n0;JMP\n(true%d)\nD=-1\n(end%d)\n%s\n";
		wr = snprintf((*instr), 512, format, POP_D, POP_D, jmp_c, jmp_c, jmp_c, jmp_c, PUSH_D);
		jmp_c++;
	}

	return wr;
}

/**
	Writes the push operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@param file: the name of the file containing the instruction
	@return the number of characters written
*/
int write_push(char **tokens, char **instruction, char *file){
	int wr = -1;
	if(strcmp(tokens[1], "local") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=M\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "LCL", PUSH_D);
	}
	else if(strcmp(tokens[1], "argument") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=M\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "ARG", PUSH_D);
	}
	else if(strcmp(tokens[1], "this") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=M\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "THIS", PUSH_D);
	}
	else if(strcmp(tokens[1], "that") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=M\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "THAT", PUSH_D);
	}
	else if(strcmp(tokens[1], "pointer") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "3", PUSH_D);
	}
	else if(strcmp(tokens[1], "temp") == 0){
		char format[100] = "@%s\nD=A\n@%s\nA=D+A\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], "5", PUSH_D);
	}
	else if(strcmp(tokens[1], "static") == 0){
		char name[32];
		memset(name, 0, 32);
		memcpy(name, file, strlen(file)-3);
		char format[100] = "@%s.%s\nD=M\n%s\n";
		wr = snprintf((*instruction), 512, format, name, tokens[2], PUSH_D);
	}
	else if(strcmp(tokens[1], "constant") == 0){
		char format[100] = "@%s\nD=A\n%s\n";
		wr = snprintf((*instruction), 512, format, tokens[2], PUSH_D);
	}
	return wr;
}


/**
	Writes the pop operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@param file: the name of the file containing the instruction
	@return the number of characters written
*/
int write_pop(char **tokens, char **instruction, char *file){
	int wr = -1;
	if(strcmp(tokens[1], "local") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nA=M\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "LCL");
	}
	else if(strcmp(tokens[1], "argument") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nA=M\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "ARG");
	}
	else if(strcmp(tokens[1], "this") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nA=M\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "THIS");
	}
	else if(strcmp(tokens[1], "that") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nA=M\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "THAT");
	}
	else if(strcmp(tokens[1], "pointer") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "3");
	}
	else if(strcmp(tokens[1], "temp") == 0){
		char format[100] = "%s\n@R13\nM=D\n@%s\nD=A\n@%s\nD=D+A\n@R14\nM=D\n@R13\nD=M\n@R14\nA=M\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, tokens[2], "5");
	}
	else if(strcmp(tokens[1], "static") == 0){
		char name[32];
		memset(name, 0, 32);
		memcpy(name, file, strlen(file)-3);
		char format[100] = "%s\n@%s.%s\nM=D\n";
		wr = snprintf((*instruction), 512, format, POP_D, name, tokens[2]);
	}
	return wr;
}

/**
	Writes the label operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_label(char **tokens, char **instruction){
	int wr = -1;
	if(!check_label(tokens[1])){
		return -1;
	}
	char format[32] = "(%s$%s)\n";
	wr = snprintf((*instruction), 512, format, current_function, tokens[1]);
	return wr;
}

/**
	Writes the goto operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_goto(char **tokens, char **instruction){
	int wr = -1;

	char format[32] = "@%s$%s\n0;JMP\n";
	wr = snprintf((*instruction), 512, format, current_function, tokens[1]);
	return wr;
}

/**
	Writes the if-goto operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_if(char **tokens, char **instruction){
	int wr = -1;

	char format[32] = "%s\n@%s$%s\nD;JNE\n";
	wr = snprintf((*instruction), 512, format, POP_D, current_function, tokens[1]);
	return wr;
}

/**
	Writes the function operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_function(char **tokens, char **instruction){
	int wr = -1;
	if(!check_label(tokens[1])){
		printf("in check\n");
		return -1;
	}

	char *end;
	long num;

	num = strtol(tokens[2], &end, 10);

	char init[33] = "D=0\n@SP\nA=M\nM=D\n@SP\nM=M+1\n";
	char format[32] = "(%s)\n";

	char *declaration = (char*)malloc(64);

	int length = snprintf(declaration, 64, format, tokens[1]);
	for(int i = 0; i < num; i++){
		declaration = (char*) realloc(declaration, length + 32);
		if(declaration == NULL){
			return -1;
		}

		strcat(declaration, init);
		length += 32;
		printf("len %d\n", length);
	}
	declaration = (char*)realloc(declaration, length+1);
	if(declaration == NULL){
			return -1;
	}
	declaration[length] = 0;

	wr = snprintf((*instruction), 512, declaration);
	
	set_current(tokens[1]);
	return wr;
}

/**
	Writes the return operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_return(char **tokens, char **instruction){
	int wr = -1;
	char format[300] = "@LCL\nD=M\n@R13\nM=D\n@5\nD=D-A\nA=D\nD=M\n@R14\nM=D\n%s\n@ARG\nA=M\nM=D\n@ARG\nD=M\n@SP\nM=D+1\n@R13\nD=M\n@1\nD=D-A\n"\
						"A=D\nD=M\n@THAT\nM=D\n@R13\nD=M\n@2\nD=D-A\nA=D\nD=M\n@THIS\nM=D\n@R13\nD=M\n@3\nD=D-A\nA=D\nD=M\n@ARG\n"\
						"M=D\n@R13\nD=M\n@4\nD=D-A\nA=D\nD=M\n@LCL\nM=D\n@R14\nA=M\n0;JMP\n";
	wr = snprintf((*instruction), 512, format, POP_D);
	//pop(&current_function);
	return wr;
}

/**
	Writes the call operation.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@return the number of characters written
*/
int write_call(char **tokens, char **instruction){
	int wr = -1;
	char format[200] = "@%s_return%d\nD=A\n%s\n@LCL\nD=M\n%s\n@ARG\nD=M\n%s\n@THIS\nD=M\n%s\n@THAT\nD=M\n%s\n"\
						"@SP\nD=M\n@5\nD=D-A\n@%s\nD=D-A\n@ARG\nM=D\n@SP\nD=M\n@LCL\nM=D\n@%s\n0;JMP\n(%s_return%d)\n";
	wr = snprintf((*instruction), 512, format, current_function, jmp_c, PUSH_D, PUSH_D, PUSH_D, PUSH_D, PUSH_D,
					 tokens[2], tokens[1], current_function, jmp_c);
	jmp_c++;
	return wr;
}

/**
	Chooses which instruction to write and calls the needed function.

	@param tokens: array of strings containing the vm instruction
	@param instruction: the string to which to write the instruction
	@param file: the name of the file containing the instruction
	@return the number of characters written
*/
int compile(char **tokens, char **instruction, char *file){
	enum command_t cmd_type = get_command_type(tokens);

	int ind = -1;
	if((ind = index_of(file, '/')) != -1){
		file = file+ind+1;
	}

	(*instruction) = (char*)malloc(512);
	switch(cmd_type){
		case C_ARITHMETIC:
			return write_arithmetic(tokens[0], instruction);
		case C_PUSH:
			return write_push(tokens, instruction, file);
		case C_POP:
			return write_pop(tokens, instruction, file);
		case C_LABEL:
			return write_label(tokens, instruction);
		case C_GOTO:
			return write_goto(tokens, instruction);
		case C_IF:
			return write_if(tokens, instruction);
		case C_FUNCTION:
			return write_function(tokens, instruction);
		case C_RETURN:
			return write_return(tokens, instruction);
		case C_CALL:
			return write_call(tokens, instruction);
		case C_NULL:
			return -1;
	}

}