#include "../s21_string.h"

static char *savestr = s21_NULL;

int delim_check(char ch, const char *delim) {
  int res = 0;
  while (*delim != '\0') {
    if (ch == *delim) {
      res = 1;
    }
    delim++;
  }
  return res;
}

char *s21_strtok(char *str, const char *delim) {
  char *start = s21_NULL;

  if (str != s21_NULL) {
    savestr = str;
  }

  if (savestr != s21_NULL) {
    while (*savestr != '\0' && delim_check(*savestr, delim) == 1) {
      savestr++;
    }

    if (*savestr != '\0') {
      start = savestr;

      while (*savestr != '\0' && delim_check(*savestr, delim) == 0) {
        savestr++;
      }

      if (*savestr != '\0') {
        *savestr = '\0';
        savestr++;
      } else {
        savestr = s21_NULL;
      }
    } else {
      savestr = s21_NULL;
    }
  }

  return start;
}