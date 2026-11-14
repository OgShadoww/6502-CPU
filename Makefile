CC = clang
CFLAGS = -g -Wall -Wextra -O0
TARGET = main
SRC = main.c

$(TARGET): Makefile $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
