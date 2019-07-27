CC=gcc
CFLAGS=-Wall -Wextra -O3
LDFLAGS=-lSDL2
OUT=out
ROM=pong2.bin

all:
	$(CC) $(CFLAGS) main.c -o $(OUT) $(LDFLAGS)

test: all
	./$(OUT) $(ROM)

clean:
	-rm -f $(BIN_DIS)
	-rm -f $(BIN)

.PHONY: clean all test


