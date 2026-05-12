CC = gcc
CFLAGS = -Wall -g -Iinclude

SRC = src/main.c
OUT = poker.exe

all:
	$(CC) $(CFLAGS) $(SRC) -o $(OUT)

run: all
	./$(OUT)

clean:
	del /Q $(OUT)