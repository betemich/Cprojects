#include "../s21_string.h"

void *s21_memset(void *str, int ch, s21_size_t n) {
  for (s21_size_t i = 0; i < n; i++) {
    *((char *)str + i) = ch;
  }

  return str;
}