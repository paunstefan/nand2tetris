

#ifndef PARSER_H
#define PARSER_H

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

void strip_comments(char **line);
int tokenize(char *line, char ***tokens);
enum command_t get_command_type(char **tokens);

#endif