#include "../s21_string.h"

char *s21_strpbrk(const char *str1, const char *str2) {
  char *res = s21_NULL;
  int match = 0;
  for (s21_size_t i = 0; i < s21_strlen(str1) && match == 0; i++) {
    for (s21_size_t k = 0; k < s21_strlen(str2) && match == 0; k++) {
      if (str1[i] == str2[k]) {
        res = (char *)&str1[i];
        match = 1;
      }
    }
  }
  return res;
}