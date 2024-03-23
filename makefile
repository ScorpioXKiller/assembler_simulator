CC = gcc
OBJS = main.o preprocessor.o
EXEC = main
DEBUG_FLAG = -g
COMP_FLAG = -Wall -ansi -pedantic $(DEBUG_FLAG)

$(EXEC): $(OBJS)
	$(CC) $(DEBUG_FLAG) $(OBJS) -o $@

main.o: main.c preprocessor.h
	$(CC) -c $(COMP_FLAG) $*.c

preprocessor.o: preprocessor.c preprocessor.h
	$(CC) -c $(COMP_FLAG) $*.c

clean:
	rm -f $(OBJS)
