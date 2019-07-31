CC=gcc
CFLAGS=-Wall -Wextra -O3
LDFLAGS=-lSDL2
OUT=out
ROM=pong2.bin

all: $(OUT)

$(OUT): main.c helpers.c helpers.h
	$(CC) $(CFLAGS) main.c helpers.c -o $(OUT) $(LDFLAGS)

test: all
	./$(OUT) $(ROM)

clean:
	rm -f $(OUT)

.PHONY: all clean test

