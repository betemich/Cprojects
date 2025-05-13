#include "../s21_string.h"

char *s21_strncat(char *dest, const char *src, s21_size_t n) {
  int len = s21_strlen(dest);

  for (s21_size_t i = 0 && src[i] != '\0'; i < n; i++) {
    dest[len++] = src[i];
  }

  dest[len] = '\0';

  return dest;
}