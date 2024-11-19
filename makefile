TARGET = main

SRC = main.c

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lssl -lcrypto -lm

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)