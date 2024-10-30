CC = gcc
OBJS = main.o preprocessor.o first_pass.o second_pass.o backend.o converter.o parser.o lexer.o consts.o utils.o
EXEC = main
DEBUG_FLAG = -g
COMP_FLAG = -Wall -ansi -pedantic $(DEBUG_FLAG)

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

main.o: main.c preprocessor.h assembler.h backend.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

preprocessor.o: preprocessor.c preprocessor.h utils.h consts.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

first_pass.o: first_pass.c assembler.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

second_pass.o: second_pass.c assembler.h converter.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

backend.o: backend.c backend.h utils.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

converter.o: converter.c converter.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

parser.o: parser.c parser.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

lexer.o: lexer.c lexer.h consts.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

utils.o: utils.c utils.h errors.h
	$(CC) -c $(COMP_FLAG) $*.c

consts.o: consts.c consts.h
	$(CC) -c $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS)