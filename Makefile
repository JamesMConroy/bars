.POSIX:
.PHONY: all clean example

CC=clang

all: bars

bars: bars.c
	$(CC) -std=c11 -lm -Werror -Wall -pedantic -o $(@) $(^)

example: bars
	./bars -c 80 < example
	./bars example

clean:
	rm bars

