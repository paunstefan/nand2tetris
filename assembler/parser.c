#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parser.h"
#include "code.h"
#include "hashtable.h"
#include "global.h"



/**
	Removes all whitespace and comments from a line.

	@param line: the line string which will be edited in place
*/
void strip(char** line){
	char *ln = (char*)malloc(sizeof(char) * 100);
	memset(ln, 0, sizeof(char) * 100);

	for(int i = 0, j = 0; (*line)[i] != '\0'; i++){
		if((*line)[i] == '\n' || (*line)[i] == '\r' ||((*line)[i] == '/' && (*line)[i+1] == '/')){
			break;
		}
		if((*line)[i] == ' ' || (*line)[i] == '\t'){
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
	Returns the command type.

	@param line: the command
	@return the command
*/
enum cmd_type process(char** line){
	enum cmd_type command_type;

	strip(line);
	if((*line)[0] == '@'){
		command_type = A_COMMAND;
	}
	else if((*line)[0] == '('){
		command_type = L_COMMAND;
	}
	else{
		command_type = C_COMMAND;
	}

	return command_type;
}


/**
	Constructs the binary instruction from an input command line.

	@param line: the command
	@param binary: the string where to write the instruction
	@param type: the command type
	@param free_mem: the first free memory space
	@return true if it is a valid instruction, else false
*/
bool get_binary(char* line, char** binary, enum cmd_type type, uint16_t *free_mem){
	//initialize();
	if(type == A_COMMAND){
		uint16_t value = 0;
		size_t len = strlen(line);
		uint16_t pow = 1;

		if(line[1] >= '0' && line[1] <='9'){

			for(int i = len - 1; i > 0; i--){
				int nr = line[i] - '0';
				if(nr > 9 || nr < 0){
					return false;
				}
				value += nr * pow;
				pow *= 10;
			}
		}
		// If a label was used
		else{
			char *label = (char*)malloc(sizeof(char*) * 64);

			strcpy(label, line+1);

			if(is_item(symbol_table, label)){
				value = get_item(symbol_table, label);
			}
			else{
				insert_HT(symbol_table, label, *free_mem);
				value = *free_mem;
				(*free_mem)++;
			}
			free(label);

		}


		compile_A(value, binary);
		return true;
	}// end A_COMMAND

	else if(type == C_COMMAND){
		(*binary)[0] = '1';
		(*binary)[1] = '1';
		(*binary)[2] = '1';

		int jmp_index;
		int comp_index;

		// Check if there is a JMP instruction
		if((jmp_index =index_of(line, ';')) != -1){
			int i = 0;
			char* temp = line + jmp_index + 1;
			char* mnemonic = (char*)malloc(sizeof(char) * 5);
			while(temp[i] != '\0'){
				mnemonic[i] = temp[i];
				i++;
			}
			mnemonic[i] = '\0';
			uint16_t value = 0;
			if(!is_item(jmp_table,mnemonic)){
				return false;
			}
			value = get_item(jmp_table, mnemonic);
			get_jmp(value, binary);
			free(mnemonic);
		}
		else{
			get_jmp(0, binary);
		}

		// Check if there is a DST instruction
		if((comp_index =index_of(line, '=')) != -1){
			int i = 0;
			char* temp = line;
			char* mnemonic = (char*)malloc(sizeof(char) * 5);
			while(temp[i] != '='){
				mnemonic[i] = temp[i];
				i++;
			}
			mnemonic[i] = '\0';
			uint16_t value = 0;
			if(!is_item(dest_table, mnemonic)){
				return false;
			}
			value = get_item(dest_table, mnemonic);
			get_dst(value, binary);
			free(mnemonic);
		}
		else{
			get_dst(0, binary);
		} // end DST parse

		// Get COMP instruction
		int i = 0;
		char *temp = line + comp_index + 1;
		char *mnemonic = (char*)malloc(sizeof(char) * 5);
		while(temp[i] != ';' && temp[i] != '\0'){
			mnemonic[i] = temp[i];
			i++;
		}
		mnemonic[i] = '\0';
		uint16_t value = 0;
		if(!is_item(comp_table, mnemonic)){
			return false;
		}
		value = get_item(comp_table, mnemonic);
		get_comp(value, binary);
		free(mnemonic);

		return true;

	}// end C_COMMAND

	return false;
}

