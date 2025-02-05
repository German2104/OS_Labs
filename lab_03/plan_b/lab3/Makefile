COMP = gcc
FILES = SystemErrors/errors.c


all: main child

main: main.c
	$(COMP) main.c $(FILES) -o main

child: child.c
	$(COMP) child.c $(FILES) -o child

clean:
	rm -f main
	rm -f child
