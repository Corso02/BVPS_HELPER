# -*- MakeFile -*-
CC= gcc
CFLAGS= -std=c11 -Wall -Werror
LDLIBS= -lcrypt
OUTPUT= zadanie2
SOURCES= zadanie2.c

all: zadanie2

zadanie2: zadanie2.o 
	$(CC) $^ $(LDLIBS) -o $(OUTPUT)

zadanie2.o: zadanie2.c
	$(CC) $(CFLAGS) -c $^ -o $@

gen: gen.o
	$(CC) $^ $(LDLIBS) -o generator
gen.o: file_gen.c
	$(CC) $(CFLAGS) -c $^ -o $@

vgrindGen: generator
	valgrind --tool=memcheck --track-origins=yes ./$^
	
vgrind: $(OUTPUT)
	valgrind --tool=memcheck ./$^

clean:
	rm -f *.o
