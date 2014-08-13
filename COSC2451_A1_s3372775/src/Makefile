CC = clang
CFLAGS = -Wall #-std=c99 -g -pg
SOURCES = pacman.c interface.c IO.c
OBJS = $(patsubst %.c,%.o,$(SOURCES))
HEADERS = interface.h IO.h
	
pacman:	$(OBJS)
	$(CC) $(CFLAGS) -o pacman $(OBJS) -lm -lncurses

$(OBJS): $(HEADERS)

.PHONY: clean
clean:
	rm  *.o 
	
.PHONY: cleanall
cleanall:
	rm  pacman *.o
