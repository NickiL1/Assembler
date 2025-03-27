CC = gcc
CFLAGS = -ansi -Wall -pedantic  -fsanitize=address -g
EXE_DEPS =  assembler.o preassem.o firstpass.o secondpass.o errors.o util.o lexer.o datastrct.o table.o

assembler: $(EXE_DEPS) src/assembler.c
	$(CC)  $(EXE_DEPS) $(CFLAGS) -o $@

assembler.o: src/assembler.c
	$(CC) -c src/assembler.c $(CFLAGS) -o $@

preassem.o: src/preassem.c
	$(CC) -c src/preassem.c $(CFLAGS) -o $@

firstpass.o: src/firstpass.c
	$(CC) -c src/firstpass.c $(CFLAGS) -o $@

secondpass.o: src/secondpass.c
	$(CC) -c src/secondpass.c $(CFLAGS) -o $@

errors.o: src/errors.c
	$(CC) -c src/errors.c $(CFLAGS) -o $@

util.o: src/util.c
	$(CC) -c src/util.c $(CFLAGS) -o $@
	
lexer.o: src/lexer.c
	$(CC) -c src/lexer.c $(CFLAGS) -o $@

datastrct.o: src/datastrct.c
	$(CC) -c src/datastrct.c $(CFLAGS) -o $@

table.o: src/table.c
	$(CC) -c src/table.c $(CFLAGS) -o $@

clean: 
	rm -rf *.o
