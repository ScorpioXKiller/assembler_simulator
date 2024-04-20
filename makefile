CC = gcc
OBJS = main.o preprocessor.o converter.o parser.o utils.o lexer.o 
EXEC = main
DEBUG_FLAG = -g
COMP_FLAG = -Wall -ansi -pedantic $(DEBUG_FLAG)

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

main.o: main.c preprocessor.h converter.h parser.h lexer.h
	$(CC) -c $(COMP_FLAG) $*.c

preprocessor.o: preprocessor.c preprocessor.h parser.h utils.h
	$(CC) -c $(COMP_FLAG) $*.c

converter.o: converter.c converter.h
	$(CC) -c $(COMP_FLAG) $*.c

parser.o: parser.c parser.h utils.h lexer.h
	$(CC) -c $(COMP_FLAG) $*.c

lexer.o: lexer.c lexer.h
	$(CC) -c $(COMP_FLAG) $*.c

utils.o: utils.c utils.h
	$(CC) -c $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS)