#include "../s21_string.h"

char *s21_strchr(const char *str, int ch) {
  char *res = s21_NULL;
  int match = 0;

  for (s21_size_t i = 0; i < s21_strlen(str) && match == 0; i++) {
    if (str[i] == ch) {
      res = (char *)&str[i];
      match = 1;
    }
  }

  return res;
}