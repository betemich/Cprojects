#include "../s21_string.h"

char *s21_strrchr(const char *str, int ch) {
  char *res = s21_NULL;

  s21_size_t len = s21_strlen(str);
  for (s21_size_t i = 0; i <= len; i++) {
    if (str[i] == ch) {
      res = (char *)(str + i);
    }
  }

  return res;
}