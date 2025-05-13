#include "../s21_string.h"

void *s21_to_upper(const char *str) {
  if (str == s21_NULL) {
    return s21_NULL;
  }

  s21_size_t len = s21_strlen(str);
  char *result = malloc((len + 1) * sizeof(char));
  if (result == s21_NULL) {
    return s21_NULL;
  }

  result = s21_strncpy(result, str, len + 1);
  if (result == s21_NULL) {
    return s21_NULL;
  }

  for (s21_size_t i = 0; i < len; i++) {
    if (result[i] >= 'a' && result[i] <= 'z') {
      result[i] -= ('a' - 'A');
    }
  }
  return result;
}

void *s21_to_lower(const char *str) {
  if (str == s21_NULL) {
    return s21_NULL;
  }

  s21_size_t len = s21_strlen(str);
  char *result = malloc((len + 1) * sizeof(char));
  if (result == s21_NULL) {
    return s21_NULL;
  }

  result = s21_strncpy(result, str, len + 1);
  if (result == s21_NULL) {
    return s21_NULL;
  }

  for (s21_size_t i = 0; i < len; i++) {
    if (result[i] >= 'A' && result[i] <= 'Z') {
      result[i] += ('a' - 'A');
    }
  }
  return result;
}

void *s21_insert(const char *src, const char *str, size_t start_index) {
  if (src == s21_NULL || str == s21_NULL) {
    return s21_NULL;
  }

  s21_size_t src_len = s21_strlen(src);
  s21_size_t str_len = s21_strlen(str);

  if (start_index > src_len) {
    return s21_NULL;
  }

  char *result = malloc(str_len + src_len + 1);
  if (result == s21_NULL) {
    return s21_NULL;
  }

  s21_size_t i = 0;
  while (i < start_index) {
    result[i] = src[i];
    i++;
  }
  for (s21_size_t j = 0; j < str_len; j++) {
    result[i++] = str[j];
  }
  for (s21_size_t j = start_index; j < src_len; j++) {
    result[i++] = src[j];
  }
  result[i] = '\0';

  return (void *)result;
}

void *s21_trim(const char *src, const char *trim_chars) {
  if (src == s21_NULL || trim_chars == s21_NULL) {
    return s21_NULL;
  }

  int left = 0;
  int right = (int)s21_strlen(src) - 1;

  while (left <= right) {
    s21_size_t j = 0;
    while (trim_chars[j] != '\0' && trim_chars[j] != src[left]) {
      j++;
    }
    if (trim_chars[j] == '\0') {
      break;
    }
    left++;
  }

  while (right >= left) {
    s21_size_t j = 0;
    while (trim_chars[j] != '\0' && trim_chars[j] != src[right]) {
      j++;
    }
    if (trim_chars[j] == '\0') {
      break;
    }
    right--;
  }

  s21_size_t result_len = 0;
  if (right - left >= 0) {
    result_len = right - left + 1;
  } else {
    result_len = 0;
  }

  char *result = malloc(result_len + 1);
  if (result == s21_NULL) {
    return s21_NULL;
  }

  for (s21_size_t i = 0; i < result_len; i++) {
    result[i] = src[left++];
  }
  result[result_len] = '\0';

  return result;
}