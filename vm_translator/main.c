#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include "parser.h"
#include "codewriter.h"

void help(){
	printf("Usage: ./vm_translator [VM file/directory] [output file]\n");
}

bool is_dir(char *name){
	int extension = strlen(name) - 3;
	if(strcmp(name+extension, ".vm") == 0){
		return false;
	}
	return true;
}

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

	char **files = NULL;
	int no_files = 0;

	if(is_dir(argv[1])){
		no_files = read_dir(argv[1], &files);
		if(no_files == 0){
			printf("No VM files found.");
			exit(1);
		}
	}
	else{
		files = (char**)malloc(sizeof(char*));
		files[0] = (char*)malloc(strlen(argv[1]) + 1);
		strcpy(files[0], argv[1]);
		no_files = 1;
	}


	for(int i = 0; i < no_files; i++){
		printf("%s\n", files[i]);
	}

	FILE* vm_fp;
	FILE* asm_fp;

	asm_fp = fopen(argv[2], "w");
	if(asm_fp == NULL){
		printf("Cannot create assembly file.\n");
		exit(1);
	}

	for(int i = 0; i < no_files; i++){
		vm_fp = fopen(files[i], "r");
		if(vm_fp == NULL){
			printf("Cannot open VM file %s.\n", files[i]);
			exit(1);
		}

		char *line = NULL;
		size_t len = 0;
		ssize_t read;

		int line_count = 0;

		while((read = getline(&line, &len, vm_fp)) != -1){
			line_count++;
			strip_comments(&line);

			char **tokens = NULL;
			int no_tokens = tokenize(line, &tokens);

			if(no_tokens == 0){
				continue;
			}

			char *instructions = NULL;
			int stat = compile(tokens, &line, files[i]);
			if(stat == -1){
				printf("Error in %s:%d.", files[i], line_count);
			}

			//write to file

		}

		fclose(vm_fp);
	}
	fclose(asm_fp);
}