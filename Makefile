CFLAGS=-std=c99 -Wall -pedantic -g

compiler: scanner.o symtable.o interpret.o parser.o main.o
	gcc $(CFLAGS) scanner.o symtable.o interpret.o parser.o main.o -lm -o compiler

scanner.o: scanner.c
	gcc $(CFLAGS) -c scanner.c

symtable.o: symtable.c
	gcc $(CFLAGS) -c symtable.c

parser.o: parser.c
	gcc $(CFLAGS) -c parser.c

interpret.o: interpret.c
	gcc $(CFLAGS) -c interpret.c

tags.o: tags.c
	gcc $(CFLAGS) -c tags.c

main.o: main.c
	gcc $(CFLAGS) -c main.c

clean:
	rm -f *.o
