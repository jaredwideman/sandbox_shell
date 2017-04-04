obj-m += batt_stat.o
obj-m += keylogger.o

all: better_shell.o
	cc -o better_shell better_shell.o
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
better_shell.o: better_shell.c
	cc -c better_shell.c

clean:
	rm -f *.o better_shell
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
