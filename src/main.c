#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>
#include <limits.h>
#include <pwd.h>
#include "config.h"

#define SLSH_TOK_BUFSIZE 64
#define SLSH_TOK_DELIM " \t\r\n\a"
#define SLSH_RL_BUFSIZE 1024

/*
	Function declarations for default shell commands 
*/
int slsh_cd(char **args);
int slsh_help(char **args);
int slsh_exit(char **args);

/*
 	List of builtin commands with their functions
*/
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&slsh_cd,
	&slsh_help,
	&slsh_exit
};

int slsh_execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		// Empty command was returned
		return 1;
	}

	for (i = 0; i < slsh_num_builtins(); i++) {
		if (strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i]) (args);
		}
	}

	return slsh_launch(args);
}


int slsh_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

/*
 	Builtin function implementations
*/
int slsh_cd(char **args)
{
	const char *homedir;

	if(chdir(args[1]) == getenv("HOME")) {
		perror("slsh");
	} else {
		if (chdir(args[1]) != 0) {
			perror("slsh");
		}
	}
	return 1;
}



int slsh_help(char **args)
{
	int i;
	printf("Beau McAdams' SLSH (Spicy Lil' SHell), a better LSH\n");
	printf("Type program names and you're off to the races!\n");
	printf("The following are built in:\n");

	for (i = 0; i < slsh_num_builtins(); i++) {
		printf("  %s\n", builtin_str[i]);
	}

	printf("Read the manual pages for info on other programs.\n");
	return 1;
}

int slsh_exit(char **args)
{
	return 0;
}


int slsh_launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if (execvp(args[0], args) == -1) {
			perror("slsh");
		}
		exit (EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("slsh");
	} else {
		// Parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}


char **slsh_split_line(char *line)
{
	int bufsize = SLSH_TOK_BUFSIZE, position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if (!tokens) {
		fprintf(stderr, "slsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, SLSH_TOK_DELIM);
	while (token != NULL) {
		tokens[position] = token;
		position++;

		if (position >= bufsize) {
			bufsize += SLSH_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if (!tokens) {
				fprintf(stderr, "slsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, SLSH_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
}


char *slsh_read_line(void)
{
	int bufsize = SLSH_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c;

	if (!buffer) {
		fprintf(stderr, "slsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while (1) {
		// Reads a character
		c = getchar();

		if (c == EOF || c == '\n') {
			buffer[position] = '\0';
			return buffer;
		} else {
		  buffer[position] = c;
		}
		position++;

		// If the buffer is exceeded, reallocate
		if (position >= bufsize) {
			bufsize += SLSH_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if (!buffer) {
				fprintf(stderr, "slsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}


void slsh_loop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf(promptsym);
		line = slsh_read_line();
		args = slsh_split_line(line);
		status = slsh_execute(args);

		free(line);
		free(args);
		} while(status);
}



int main(int argc, char **argv) {
	// Initialize config files
	
	
	slsh_loop();

	// Perform shutdown or cleanup
	return EXIT_SUCCESS;
}
