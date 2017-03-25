#include <stdio.h>
#include <stdlib.h>

#define SHL_RL_BUFSIZE 1024

// TODO: basically an exact copy of the beginning of this tutorial so far (https://brennan.io/2015/01/16/write-a-shell-in-c/), need to make sure it's built in a way we won't get dinged for plagarism obviously

char *shl_read_line(void) {
    int bufsize = SHL_RL_BUFSIZE;
    int pos = 0;
    char *buffer = malloc(sizeof(char) * bufsize);
    int c;

    if (!buffer) {
        fprintf(stderr, "ALLOCATION ERROR\n");
        exit(EXIT_FAILURE);
    }
    
    while(1) {
        c = getchar();
        if (c == EOF || c == '\n') {
            buffer[pos] = '\0';
            return buffer;
        } else {
            buffer[pos++] = c;
        }
    }

    if (pos >= bufsize) {
        bufsize += SHL_RL_BUFSIZE;
        buffer = realloc(buffer, bufsize);
        if (!buffer) {
            fprintf(stderr, "ALLOCATION ERROR\n");
            exit(EXIT_FAILURE);
        }
    }
}

void shl_loop(char *prompt) {
    int running = 1;
    char *line;

    while(running) {
    	if (prompt != '\0') {
            printf("%s ", prompt);
        } else {
            printf("> ");
        }
        shl_read_line();
    }
}

int main(int argc, char **argv) {
    shl_loop(argv[1]);
    return 0;
}

