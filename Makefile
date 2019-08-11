CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lSDL2 -lm
OUT=emul
ROM=PONG2

all: $(OUT)

$(OUT): main.c
	$(CC) $(CFLAGS) main.c -o $(OUT) $(LDFLAGS)

test: all
	./$(OUT) $(ROM)


clean:
	rm -f $(OUT)



