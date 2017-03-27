#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define KRED  "\x1B[31m"

#define SHL_RL_BUFSIZE 1024
#define SHL_TOK_BUFSIZE 64
#define SHL_TOK_DELIM " \t\r\n\a"

// TODO: basically an exact copy of the entire tutorial so far (https://brennan.io/2015/01/16/write-a-shell-in-c/), need to make sure it's built in a way we won't get dinged for plagarism obviously


// CD, HELP, EXIT all work.


// function declarations for builtin shell commands
int shl_cd(char **args);
int shl_help(char **args);
int shl_exit(char **args);

char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&shl_cd,
	&shl_help,
	&shl_exit
};

int shl_num_builtins() {
	return sizeof(builtin_str) / sizeof(char *);
}

int shl_cd(char **args) {
	if(args[1] == NULL) {
		fprintf(stderr, "shl: expected arg to \"cd\"\n");
	} else {
		if(chdir(args[1]) != 0) {
			perror("shl");
		}
	}
	return 1;
}

int shl_help(char **args) {
	int i;
	printf("Chris & Jared's SHELL\n");
	printf("The following commands are built-in:\n");

	for(i = 0; i < shl_num_builtins(); i++) {
		printf("	%s\n", builtin_str[i]);
	}
	return 1;
}


int shl_exit(char **args) {
	return 0;
}


char *shl_read_line(void) {
	char *line = NULL;
	ssize_t bufsize = 0;
	getline(&line, &bufsize, stdin);
	return line;
}

char **shl_split_line(char *line) {
	int bufsize = SHL_TOK_BUFSIZE, pos = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens) {
		fprintf(stderr, "lsh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, SHL_TOK_DELIM);
	while(token != NULL) {
		tokens[pos++] = token;

		if(pos >= bufsize) {
			bufsize += SHL_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));
			if(!tokens) {
				fprintf(stderr, "lsh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
		token = strtok(NULL, SHL_TOK_DELIM);
	}
	tokens[pos] = NULL;
	return tokens;
}

int shl_launch(char **args) {
	pid_t pid, wpid;
	int status;

	pid = fork();
	if(pid == 0) {
		//child
		if(execvp(args[0], args) == -1) {
			perror("shl");
		}
		exit(EXIT_FAILURE);

	} else if(pid < 0) {
		//error forking
		perror("shl");
	} else {
		//parent
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}
	return 1;
}


int shl_execute(char **args) {
	int i;
	if(args[0] == NULL) {
		return 1;
	}

	for(i = 0; i < shl_num_builtins(); i++) {
		if(strcmp(args[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(args);
		}
	}
	return shl_launch(args);
}



void shl_loop(void) {
    int status;
    char *line;
	char **args;
	
	printf("\n%sWELCOME TO THE BETTER SHELL\n", KRED);

    do {
		printf("> ");
		line = shl_read_line();
		args = shl_split_line(line);
		status = shl_execute(args);
		free(line);
		free(args);
	} while(status);
}

int main(int argc, char **argv) {
    shl_loop();
    return 0;
}

