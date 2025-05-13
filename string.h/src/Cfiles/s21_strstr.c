#include "../s21_string.h"

char *s21_strstr(const char *haystack, const char *needle) {
  char *res = s21_NULL;
  s21_size_t hay_len = s21_strlen(haystack);
  s21_size_t need_len = s21_strlen(needle);
  for (int i = 0; i <= (int)hay_len - (int)need_len; i++) {
    int match = 1;
    for (s21_size_t k = i, j = 0; needle[j] != '\0'; j++, k++) {
      if (haystack[k] != needle[j]) {
        match = 0;
        break;
      }
    }
    if (match == 1) {
      res = (char *)haystack + i;
      break;
    }
  }
  return res;
}