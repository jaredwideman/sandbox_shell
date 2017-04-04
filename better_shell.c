#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define TEXT_BOLD_RED  "\033[1m\033[31m"
#define TEXT_DEFAULT "\033[0m"

#define HISTORY_LENGTH 10
#define SHL_RL_BUFSIZE 1024
#define SHL_TOK_BUFSIZE 64
#define SHL_TOK_DELIM " \t\r\n\a"

// TODO: basically an exact copy of the entire tutorial so far (https://brennan.io/2015/01/16/write-a-shell-in-c/), need to make sure it's built in a way we won't get dinged for plagarism obviously


// function declarations for builtin shell commands
int shl_keylogger();
int shl_battery();
int shl_cd(char **args);
int shl_help(char **args);
int shl_exit(char **args);
int shl_history();
int add_line_to_history(char *line);
int cat_sim(char *fileparam, FILE*);

char **history;

char *builtin_str[] = {
    "keylogger",
    "battery",
    "cd",
    "help",
    "exit",
    "history"
};

int (*builtin_func[]) (char **) = {
    &shl_keylogger,
    &shl_battery,
    &shl_cd,
    &shl_help,
    &shl_exit,
    &shl_history
};

int shl_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int check_mod_existence(char *filename, FILE *fp) {
    char ch;
    int i;
    if (fp == NULL) return -1;
    return 1;
}

/* FUNCTION TO SIMULATE 'CAT' TO CALL MODULES */
int cat_sim(char *fileparam, FILE *fp) {
    char ch;
    while((ch = fgetc(fp)) != EOF) { putchar(ch); }
}

int s_insmod(char *filename) {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "sudo insmod %s", filename);
    return system(buffer);
}

int shl_keylogger() {
    FILE *fp;
    char location[30] = "/proc/keylogger";
    fp = fopen(location, "r");
    int exists = check_mod_existence(location, fp);
    if (exists == -1) {
        printf("Module doesn't exist. Automatically adding module to kernel. Please rerun command.\n");
        s_insmod("keylogger.ko");
        return -1;
    }
    int ret = cat_sim(location, fp);
    fclose(fp);
    return ret;
}


int shl_battery() {
    FILE *fp;
    char location[30] = "/proc/batt_stat";
    fp = fopen(location, "r");
    int exists = check_mod_existence(location, fp);
    if (exists == -1) {
        printf("Module doesn't exist. Automatically adding module to kernel. Please rerun command.\n");
        s_insmod("batt_stat.ko");
        return -1;
    }
    int ret = cat_sim(location, fp);
    fclose(fp);
    return ret;
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
    printf("Chris & Jared's BETTER_SHELL\n");
    printf("The following commands are built-in:\n");

    for(i = 0; i < shl_num_builtins(); i++) {
        printf("\t> %s\n", builtin_str[i]);
    }
    return 1;
}

int shl_history() {
    int i;
    for (i = HISTORY_LENGTH-1; i >= 0; i--) {
        if (strcmp(history[i], "")) {
            printf("%d. %s\n", (HISTORY_LENGTH-i), history[i]);
        }
    }
    printf("\n");
}

int shl_exit(char **args) {
    return 0;
}


char *shl_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    add_line_to_history(line);             // Add input to history
    return line;
}


/* slight variation to :

    http://www.csl.mtu.edu/cs4411.ck/www/NOTES/process/fork/exec.html 

*/
char **line_parse(char *line) {
    int pos = 0;
    char **args = malloc(100 * sizeof(char*));
    while(*line != '\0') {
        while(*line == ' ' || *line == '\t' || *line == '\n') {
            *line++ = '\0';
        }
        args[pos++] = line;
        while(*line != '\0' && *line != ' ' && *line != '\t' && *line != '\n')
            line++;
    }
    args[pos] = '\0';
    return args;
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
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
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

int add_line_to_history(char *line) {
    int i;
    char tmp_line[255];    

    strcpy(tmp_line, line);
    tmp_line[strlen(tmp_line)-1] = 0;

    for (i = HISTORY_LENGTH-1; i >= 0; --i) {
        if (strcmp(history[i], "") == 0) {
            strcpy(history[i], tmp_line);
            return 1;
        } 
    }
    for (i = HISTORY_LENGTH-1; i >= 1; --i) {
        strcpy(history[i], history[i-1]);
    }
    strcpy(history[0], tmp_line);
    return 1;
}

void shl_loop() {
    int status;
    char *line;

    char **args;
	char cwd[1024];

    int i;

	printf("\e[1;1H\e[2JWELCOME TO THE " TEXT_BOLD_RED "BETTER " TEXT_DEFAULT "SHELL\n\n");

    do {
        if (getcwd(cwd, sizeof(cwd)) == NULL) {
            perror("getcwd() error");
            return;
        }
        char *prompt;
        prompt = strcat(cwd, " ->");
        fprintf(stdout, TEXT_BOLD_RED "%s " TEXT_DEFAULT, prompt);
        line = shl_read_line();                         // Read input
        //args = shl_split_line(line);
        args = line_parse(line);        
        status = shl_execute(args);
        free(line);
        free(args);
	} while(status);
    
    // Free history
    for (i = 0; i < HISTORY_LENGTH; i++) {
        free(history[i]);
    } free(history);
}

int main(int argc, char **argv) {
    history = (char **) calloc (HISTORY_LENGTH, sizeof(char*));
    int i;
    for (i = 0; i < HISTORY_LENGTH; i++) {
        history[i] = (char*) calloc (255, sizeof(char));
    }
    shl_loop();
    return 0;
}

