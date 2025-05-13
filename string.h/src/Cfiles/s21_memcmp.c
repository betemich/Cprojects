#include "../s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  int res = 0;

  for (s21_size_t i = 0; i < n && res == 0; i++) {
    if (*((char *)str1 + i) < *((char *)str2 + i)) {
      res = -1;
    }
    if (*((char *)str1 + i) > *((char *)str2 + i)) {
      res = 1;
    }
  }

  return res;
}
