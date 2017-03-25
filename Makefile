better_shell: better_shell.o
	cc -o better_shell better_shell.o

better_shell.o: better_shell.c
	cc -c better_shell.c

clean:
	rm -f *.o better_shell
