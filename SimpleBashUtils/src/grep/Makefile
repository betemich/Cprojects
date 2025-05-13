CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRC = s21_grep.c
OBJ = s21_grep.o
BIN = s21_grep

all: $(BIN)

$(BIN): $(OBJ)
	$(CC)  $< -o $@ 

$(OBJ): $(SRC)
	$(CC) $(CFLAGS)  -c $< -o $@ 

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean