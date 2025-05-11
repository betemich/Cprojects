#include "../s21_string.h"

s21_size_t s21_strcspn(const char *str1, const char *str2) {
  s21_size_t count = 0;
  int ok = 1;
  s21_size_t str2_len = s21_strlen(str2);
  while (str1[count] != '\0' && ok) {
    for (size_t i = 0; i < str2_len; i++) {
      if (str1[count] == str2[i]) {
        ok = 0;
        break;
      }
    }
    if (!ok) break;
    count++;
  }

  return count;
}