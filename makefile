all: hash.o memsys.o
	clang -std=c99 -Wall -pedantic hash.o memsys.o -o hash

hash.o: hash.c memsys.c hash.h memsys.h
	clang -std=c99 -Wall -pedantic -c hash.c memsys.c

clean: 
	rm *.o hash
