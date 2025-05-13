#ifndef S21_CAT_H
#define S21_CAT_H

#include <assert.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1000000
#define TAB "^I"

typedef struct Arguments {
  int b, e, n, s, t, v;
  int print_file_name;
  int number;
  int slashN;
} Arguments;

void processFile(const char *file_path, Arguments *args, int first);
void printNumber(Arguments *args);
void defaultPrint(unsigned char c);
void printV(unsigned char c);
int checkS(unsigned char c, int *empty, int new_file, const Arguments *args);
int checkN(const int *new_file, const Arguments *args);
int checkB(unsigned char c, const int *new_file, const Arguments *args);
int parseArguments(int argc, char **argv, Arguments *args);

#endif
