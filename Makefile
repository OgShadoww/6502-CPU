CC = clang
CFLAGS = -g -Wall -Wextra -O0
TARGET = main
SRC = src/main.c src/cpu.c src/bus.c src/memory.c src/debug.c

$(TARGET): Makefile $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
