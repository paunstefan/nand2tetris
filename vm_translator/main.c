#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "parser.h"
#include "codewriter.h"
#include "global.h"


int jmp_c;
char *current_function;

#define set_SP "@256\nD=A\n@SP\nM=D\n"

/**
	Edits the current_function global variable.

	@param f: the current function name
*/
void set_current(char *f){
	free(current_function);
	current_function = (char*)malloc(strlen(f) + 1);
	strcpy(current_function, f);
}

/**
	Prints the help string.
*/
void help(){
	printf("Usage: ./vm_translator [VM file/directory] [output file]\n");
}

/**
	Checks if a file is a directory.

	@param name: file name
	@return true if it is a directory, else false
*/
bool is_dir(char *name){
	int extension = strlen(name) - 3;
	if(strcmp(name+extension, ".vm") == 0){
		return false;
	}
	return true;
}

/**
	Reads the files in a directory.

	@param directory: the directory name
	@param files: array of strings where the names will be written
	@return the number of files in the directory
*/
int read_dir(char *directory, char ***files){
	int count = 0;
	struct dirent *p_dirent;
	DIR *p_dir;
	p_dir = opendir(directory);
	if(p_dir == NULL){
		printf("Cannot open directory %s.", directory);
		return 0;
	}

	while((p_dirent = readdir(p_dir)) != NULL){
		if(strcmp(p_dirent->d_name, ".") == 0 || strcmp(p_dirent->d_name, "..") == 0){
			continue;
		}
		if(strcmp(p_dirent->d_name + (strlen(p_dirent->d_name) - 2), "vm")){
			continue;
		}
		count++;
		(*files) = (char**)realloc((*files), sizeof(char*) * count);
		(*files)[count-1] = (char*)malloc(strlen(p_dirent->d_name) + 1);
		strcpy((*files)[count-1], p_dirent->d_name);
	}
	closedir(p_dir);
	return count;

}


int main(int argc, char **argv){
	if(argc != 3){
		help();
		exit(1);
	}

	set_current("_default");

	char **files = NULL;
	int no_files = 0;

	if(is_dir(argv[1])){
		no_files = read_dir(argv[1], &files);
		if(no_files == 0){
			printf("No VM files found.");
			exit(1);
		}

		for(int i = 0; i < no_files; i++){

			char *file = (char*)malloc(64);
			strcpy(file, argv[1]);
			strcpy(file + strlen(argv[1]), "/");
			file = strcat(file, files[i]);
			char *temp = files[i];
			files[i] = file;
			free(temp);
		}
	}
	else{
		files = (char**)malloc(sizeof(char*));
		files[0] = (char*)malloc(strlen(argv[1]) + 1);
		strcpy(files[0], argv[1]);
		no_files = 1;
	}

	FILE* vm_fp;
	FILE* asm_fp;

	asm_fp = fopen(argv[2], "w");
	if(asm_fp == NULL){
		printf("Cannot create assembly file.\n");
		exit(1);
	}

	for(int i = 0; i < no_files; i++){
		jmp_c = 0;
		vm_fp = fopen(files[i], "r");
		if(vm_fp == NULL){
			printf("Cannot open VM file %s.\n", files[i]);
			exit(1);
		}

		char *line = NULL;
		size_t len = 0;
		ssize_t read;

		int line_count = 0;

		// bootstrap code
		if(no_files > 1){
			fprintf(asm_fp, "%s", set_SP);

			char **boot_tokens = NULL;
			char *l = (char*)malloc(16);
			strcpy(l, "call Sys.init 0");
			tokenize(l, &boot_tokens);

			char *bootstrap = NULL;
			compile(boot_tokens, &bootstrap, files[0]);
			fprintf(asm_fp, "%s", bootstrap);
			free(l);
			free(bootstrap);
		}

		while((read = getline(&line, &len, vm_fp)) != -1){
			line_count++;
			strip_comments(&line);
			char **tokens = NULL;
			int no_tokens = tokenize(line, &tokens);

			if(no_tokens == 0){
				continue;
			}

			char *instructions = NULL;
			int stat = compile(tokens, &instructions, files[i]);
			if(stat == -1){
				printf("Error in %s:%d.\n", files[i], line_count);
				remove(argv[2]);
				exit(1);
			}
			printf("%s", instructions);
			fprintf(asm_fp, "%s", instructions);
			free(instructions);

			free(tokens);

		}

		fclose(vm_fp);
	}
	fclose(asm_fp);
}