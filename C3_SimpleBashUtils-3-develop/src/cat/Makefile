CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRC = s21_cat.c
OBJ = s21_cat.o
BIN = s21_cat

all: $(BIN)

$(BIN):  $(OBJ)
	$(CC) $< -o $@

$(OBJ): $(SRC)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean