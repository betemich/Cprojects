#ifndef S21_GREP_H
#define S21_GREP_H

#include <ctype.h>
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1000000

typedef struct Arguments {
  int e, i, v, c, l, n, h, s, f, o;
  char *pattern;
  int curr_len_pattern;
  int max_len_pattern;
  int print_file_name;
} Arguments;

int getRegFromFile(Arguments *args, const char *file_path);
void printMatch(const char *line, regex_t *re, const Arguments *args,
                const char *file_path, int number_line);
void printLine(const Arguments *args, const char *line, const char *file_path,
               int number_line);
int addPattern(Arguments *args, char *pattern);
int expandPattern(Arguments *args, int size);
void fileProcess(const char *file_path, const Arguments *args, regex_t re);
int parseArguments(int argc, char **argv, Arguments *args);

#endif