CC = gcc
CFLAGS = -Wall -lm -g
#export CFLAGS
SRCS = $(wildcard *.c)

.PHONY: all clean

all: $(SRCS:.c=)

clean:
	rm -rf $(SRCS:.c=)

.c:
	$(CC) $< -o $@ $(CFLAGS)