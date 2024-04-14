CC = gcc
CFLAGS = -Wall -Wextra

SRCS := $(wildcard *.c)
BINS := $(patsubst %.c,%,$(SRCS))

.PHONY: all clean

all: $(BINS)

%: %.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	$(RM) $(BINS)