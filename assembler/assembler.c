#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include "parser.h"
#include "hashtable.h"
#include "global.h"

struct HashTable dest_table;
struct HashTable jmp_table;
struct HashTable comp_table;
struct HashTable symbol_table;

void initialize(){
	init_hash_table(&dest_table, 8);
	init_hash_table(&jmp_table, 8);
	init_hash_table(&comp_table, 28);
	init_hash_table(&symbol_table, 64);

	insert_HT(dest_table, "null", 0);
	insert_HT(dest_table, "M", 1);
	insert_HT(dest_table, "D", 2);
	insert_HT(dest_table, "MD", 3);
	insert_HT(dest_table, "A", 4);
	insert_HT(dest_table, "AM", 5);
	insert_HT(dest_table, "AD", 6);
	insert_HT(dest_table, "AMD", 7);

	insert_HT(jmp_table, "null", 0);
	insert_HT(jmp_table, "JGT", 1);
	insert_HT(jmp_table, "JEQ", 2);
	insert_HT(jmp_table, "JGE", 3);
	insert_HT(jmp_table, "JLT", 4);
	insert_HT(jmp_table, "JNE", 5);
	insert_HT(jmp_table, "JLE", 6);
	insert_HT(jmp_table, "JMP", 7);

	insert_HT(comp_table, "0", 42);
	insert_HT(comp_table, "1", 63);
	insert_HT(comp_table, "-1", 58);
	insert_HT(comp_table, "D", 12);
	insert_HT(comp_table, "A", 48);
	insert_HT(comp_table, "!D", 13);
	insert_HT(comp_table, "!A", 49);
	insert_HT(comp_table, "-D", 15);
	insert_HT(comp_table, "-A", 51);
	insert_HT(comp_table, "D+1", 31);
	insert_HT(comp_table, "A+1", 55);
	insert_HT(comp_table, "D-1", 14);
	insert_HT(comp_table, "A-1", 50);
	insert_HT(comp_table, "D+A", 2);
	insert_HT(comp_table, "D-A", 19);
	insert_HT(comp_table, "A-D", 7);
	insert_HT(comp_table, "D&A", 0);
	insert_HT(comp_table, "D|A", 21);
	insert_HT(comp_table, "M", 112);
	insert_HT(comp_table, "!M", 113);
	insert_HT(comp_table, "-M", 115);
	insert_HT(comp_table, "M+1", 119);
	insert_HT(comp_table, "M-1", 114);
	insert_HT(comp_table, "D+M", 66);
	insert_HT(comp_table, "D-M", 83);
	insert_HT(comp_table, "M-D", 71);
	insert_HT(comp_table, "D&M", 64);
	insert_HT(comp_table, "D|M", 85);

	insert_HT(symbol_table, "SP", 0);
	insert_HT(symbol_table, "LCL", 1);
	insert_HT(symbol_table, "ARG", 2);
	insert_HT(symbol_table, "THIS", 3);
	insert_HT(symbol_table, "THAT", 4);
	insert_HT(symbol_table, "R0", 0);
	insert_HT(symbol_table, "R1", 1);
	insert_HT(symbol_table, "R2", 2);
	insert_HT(symbol_table, "R3", 3);
	insert_HT(symbol_table, "R4", 4);
	insert_HT(symbol_table, "R5", 5);
	insert_HT(symbol_table, "R6", 6);
	insert_HT(symbol_table, "R7", 7);
	insert_HT(symbol_table, "R8", 8);
	insert_HT(symbol_table, "R9", 9);
	insert_HT(symbol_table, "R10", 10);
	insert_HT(symbol_table, "R11", 11);
	insert_HT(symbol_table, "R12", 12);
	insert_HT(symbol_table, "R13", 13);
	insert_HT(symbol_table, "R14", 14);
	insert_HT(symbol_table, "R15", 15);
	insert_HT(symbol_table, "SCREEN", 16384);
	insert_HT(symbol_table, "KBD", 24576);

}

void help(){
	printf("Usage: ./assembler [ASM file] [output file]\n");
}

/*
	Parses the file to add all symbols to the symbol table.
*/
void first_pass(FILE* asm_fp){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	uint32_t line_count = 0;
	uint16_t cmd_count = 0;

	while((read = getline(&line, &len, asm_fp)) != -1){
		line_count++;
		enum cmd_type cmd = process(&line);
		if(strlen(line) == 0){
			continue;
		}

		if(cmd == L_COMMAND){
			if((index_of(line, ')')) == -1){
				printf("Error in line: %d\n. Exiting...\n", line_count);
				exit(0);
			}

			char* temp = line+1;
			char* label = (char*)malloc(sizeof(char)*64);
			int i = 0;
			while(temp[i] != ')'){
				label[i] = temp[i];
				i++;
			}
			label[i] = '\0';

			insert_HT(symbol_table, label, cmd_count);
			free(label);
		}
		else{
			cmd_count++;
		}
		
	}
}

/*
	Parses the file to write the machine code.
*/
void second_pass(FILE* asm_fp, FILE* bin_fp, char** argv){
	char *line = NULL;
	size_t len = 0;
	ssize_t read;

	uint32_t line_count = 0;
	uint16_t free_mem = 16;

	while((read = getline(&line, &len, asm_fp)) != -1){
		line_count++;
		enum cmd_type cmd = process(&line);
		if(strlen(line) == 0 || cmd == L_COMMAND){
			continue;
		}

		bool check = false;

		char *binary = (char*)malloc(sizeof(char) * 18);
		memset(binary, '0', 16);
		binary[17] = '\0';
		binary[16] = '\n';
		check = get_binary(line, &binary, cmd, &free_mem);
		if(check == false){
			printf("|%s|", line);
			printf("Error on line: %d\nExiting...\n", line_count);
			remove(argv[2]);
			exit(1);
		}
		printf("%s", binary);

		fprintf(bin_fp, "%s", binary);
		free(binary);
	}
}


int main(int argc, char **argv){
	initialize();

	if(argc != 3){
		help();
		exit(1);
	}

	FILE* asm_fp;
	FILE* bin_fp;

	asm_fp = fopen(argv[1], "r");
	if(asm_fp == NULL){
		printf("ASM file does not exist.\n");
		exit(1);
	}

	bin_fp = fopen(argv[2], "w");
	if(asm_fp == NULL){
		printf("Cannot create binary file.\n");
		exit(1);
	}

	first_pass(asm_fp);
	rewind(asm_fp);
	second_pass(asm_fp, bin_fp, argv);

	fclose(asm_fp);
	fclose(bin_fp);



	return 0;
}