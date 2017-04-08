obj-m += batt_stat.o
obj-m += keylogger.o

all: sandbox_shell.o
	cc -o sandbox_shell sandbox_shell.o
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
sandbox_shell.o: sandbox_shell.c
	cc -c sandbox_shell.c

clean:
	rm -f *.o sandbox_shell
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
