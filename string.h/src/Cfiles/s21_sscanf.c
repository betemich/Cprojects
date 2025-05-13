#include "../s21_string.h"

int s21_is_space(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' ||
         c == '\r';
}

int s21_is_digit(char c) { return c >= '0' && c <= '9'; }

int s21_is_hex(char c) {
  return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
         (c >= 'A' && c <= 'F');
}

int s21_is_oct(char c) { return c >= '0' && c <= '7'; }
int s21_from_hex_char_to_int(char c) {
  int rez;
  if (c >= '0' && c <= '9') {
    rez = c - '0';
  } else if (c >= 'a' && c <= 'f') {
    rez = 10 + (c - 'a');
  } else {
    rez = 10 + (c - 'A');
  }
  return rez;
}
void s21_write_signed_or_unsigned(va_list args, const char *size,
                                  unsigned long long value, bool is_signed) {
  if (size[0] == '0') {
    if (is_signed) {
      int *argg = va_arg(args, int *);
      *argg = (int)value;
    } else {
      unsigned int *argg = va_arg(args, unsigned int *);
      *argg = (unsigned int)value;
    }
  } else if (size[0] == 'h' && size[1] != 'h') {
    if (is_signed) {
      short *argg = va_arg(args, short *);
      *argg = (short)value;
    } else {
      unsigned short *argg = va_arg(args, unsigned short *);
      *argg = (unsigned short)value;
    }
  } else if (size[0] == 'h' && size[1] == 'h') {
    if (is_signed) {
      signed char *argg = va_arg(args, signed char *);
      *argg = (signed char)value;
    } else {
      unsigned char *argg = va_arg(args, unsigned char *);
      *argg = (unsigned char)value;
    }
  } else if (size[0] == 'l' && size[1] != 'l') {
    if (is_signed) {
      long *argg = va_arg(args, long *);
      *argg = (long)value;
    } else {
      unsigned long *argg = va_arg(args, unsigned long *);
      *argg = (unsigned long)value;
    }
  } else if (size[0] == 'l' && size[1] == 'l') {
    if (is_signed) {
      long long *argg = va_arg(args, long long *);
      *argg = (long long)value;
    } else {
      unsigned long long *argg = va_arg(args, unsigned long long *);
      *argg = (unsigned long long)value;
    }
  }
}

void s21_skip(const char *str, int *str_index, bool *check_flag,
              int (*is_valid_char)(char)) {
  while (s21_is_space(str[*str_index])) {
    (*str_index)++;
  }

  if (str[*str_index] == '-') {
    (*str_index)++;
  }
  if (str[*str_index] == '+') {
    (*str_index)++;
  }

  if (!is_valid_char(str[*str_index])) {
    *check_flag = true;
  }

  while (is_valid_char(str[*str_index])) {
    (*str_index)++;
  }
}

void s21_type_s(va_list args, const char *str, int *str_index, Args arg,
                int *count) {
  if (!arg.flag) {
    char *argg = va_arg(args, char *);
    int str_arg_count = 0;
    if (arg.width == 0) {
      while (s21_is_space(str[*str_index])) {
        (*str_index)++;
      }
      while (str[*str_index] != '\0' && !s21_is_space(str[*str_index])) {
        argg[str_arg_count++] = str[(*str_index)++];
      }
    } else {
      while (str_arg_count < arg.width && str[*str_index] != '\0') {
        argg[str_arg_count++] = str[(*str_index)++];
      }
    }
    argg[str_arg_count] = '\0';
    if (str_arg_count > 0) {
      (*count)++;
    }
  } else {
    int str_arg_count = 0;
    if (arg.width == 0) {
      while (s21_is_space(str[*str_index])) {
        (*str_index)++;
      }
      while (str[*str_index] != '\0' && !s21_is_space(str[*str_index])) {
        str_arg_count++;
        (*str_index)++;
      }
    } else {
      while (str_arg_count < arg.width && str[*str_index] != '\0') {
        str_arg_count++;
        (*str_index)++;
      }
    }
  }
}

void s21_type_c(va_list args, const char *str, int *str_index, Args arg,
                int *count) {
  if (!arg.flag) {
    char *argg = va_arg(args, char *);
    int str_arg_count = 0;
    if (arg.width == 0) {
      argg[str_arg_count++] = str[(*str_index)++];
    } else {
      while (str_arg_count < arg.width && str[*str_index] != '\0') {
        argg[str_arg_count++] = str[(*str_index)++];
      }
    }
    argg[str_arg_count] = '\0';
    (*count)++;
  } else {
    int str_arg_count = 0;
    if (arg.width == 0) {
      while (s21_is_space(str[*str_index])) {
        (*str_index)++;
      }
      str_arg_count++;
      (*str_index)++;
    } else {
      while (str_arg_count < arg.width && str[*str_index] != '\0') {
        str_arg_count++;
        (*str_index)++;
      }
    }
  }
}

void s21_type_d_and_u(va_list args, const char *str, int *str_index, Args arg,
                      int *count, bool *check_d, const char *size) {
  if (!arg.flag) {
    bool minus = false;
    bool design = false;

    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }

    unsigned long long int_result = 0;
    int width_remaining = arg.width;

    if (str[*str_index] == '-') {
      minus = true;
      if (arg.width == 1) {
        *check_d = true;
        design = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '+') {
      if (arg.width == 1) {
        *check_d = true;
        design = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }

    if (!s21_is_digit(str[*str_index])) {
      *check_d = true;
    }

    while (s21_is_digit(str[*str_index]) &&
           (width_remaining > 0 || arg.width == 0)) {
      if (int_result > (ULLONG_MAX / 10) ||
          (int_result == (ULLONG_MAX / 10) &&
           (unsigned long long)(str[*str_index] - '0') > (ULLONG_MAX % 10))) {
        int_result = ULLONG_MAX;
        break;
      }
      int_result = int_result * 10 + (str[*str_index] - '0');
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }

    if (!design) {
      if (arg.type == 'd' || arg.type == 'i') {
        if (minus) {
          if (int_result > (unsigned long long)LLONG_MAX + 1) {
            int_result = (unsigned long long)LLONG_MAX + 1;
          }
          int_result = -int_result;
        } else {
          if (int_result > LLONG_MAX) {
            int_result = LLONG_MAX;
          }
        }
        s21_write_signed_or_unsigned(args, size, int_result, true);
      } else {
        if (minus && int_result < ULLONG_MAX) {
          int_result = -int_result;
        }
        s21_write_signed_or_unsigned(args, size, int_result, false);
      }
    }

    if (!*check_d) {
      (*count)++;
    }
  } else {
    s21_skip(str, str_index, check_d, s21_is_digit);
  }
}

void s21_type_o(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_o, const char *size) {
  if (!arg.flag) {
    bool minus = false;
    bool design = false;

    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }

    long long int_result = 0;
    int width_remaining = arg.width;

    if (str[*str_index] == '-') {
      minus = true;
      if (arg.width == 1) {
        *check_o = true;
        design = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '+') {
      if (arg.width == 1) {
        *check_o = true;
        design = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }

    if (!s21_is_oct(str[*str_index])) {
      *check_o = true;
    }

    while (s21_is_oct(str[*str_index]) &&
           (width_remaining > 0 || arg.width == 0)) {
      if ((unsigned long long)int_result > (ULLONG_MAX / 8)) {
        int_result = ULLONG_MAX;
        break;
      }
      int_result = int_result * 8 + (str[*str_index] - '0');
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }

    if (minus) {
      if ((unsigned long long)int_result >
          (unsigned long long)(LLONG_MAX) + 1) {
        int_result = (unsigned long long)(LLONG_MAX) + 1;
      }
      int_result = -int_result;
    }
    if (!*check_o && !design) {
      s21_write_signed_or_unsigned(args, size, int_result, false);
    }

    if (!*check_o) {
      (*count)++;
    }
  } else {
    s21_skip(str, str_index, check_o, s21_is_oct);
  }
}

void s21_type_x(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_x, const char *size) {
  if (!arg.flag) {
    bool minus = false;
    bool has_digits = false;

    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }

    long long int_result = 0;
    int width_remaining = arg.width;
    if (str[*str_index] == '-') {
      minus = true;

      if (arg.width == 1) {
        *check_x = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '+') {
      if (arg.width == 1) {
        *check_x = true;
      }
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }

    if (str[*str_index] == '0' &&
        (str[*str_index + 1] == 'x' || str[*str_index + 1] == 'X')) {
      (*str_index)++;
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
      if (arg.width > 0) width_remaining--;
      has_digits = true;
    }

    if (!s21_is_hex(str[*str_index]) && str[*str_index] != '\0' &&
        str[*str_index] != ' ') {
      *check_x = true;
    }

    while (s21_is_hex(str[*str_index]) &&
           (width_remaining > 0 || arg.width == 0)) {
      has_digits = true;
      if (int_result > (LLONG_MAX / 16) ||
          (int_result == (LLONG_MAX / 16) &&
           s21_from_hex_char_to_int(str[*str_index]) > (LLONG_MAX % 16))) {
        int_result = minus ? LLONG_MIN : LLONG_MAX;
        break;
      }
      int_result = int_result * 16 + s21_from_hex_char_to_int(str[*str_index]);
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (!has_digits) {
      *check_x = true;
    }

    if (minus) {
      int_result = -int_result;
    }

    if (!*check_x) {
      s21_write_signed_or_unsigned(args, size, int_result, false);
    }

    if (!*check_x) {
      (*count)++;
    }
  } else {
    s21_skip(str, str_index, check_x, s21_is_hex);
  }
}

void s21_type_i(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_x, bool *check_o, bool *check_d,
                const char *size) {
  bool sign_check = false;
  bool it_was_hex = false;
  bool it_was_is_oct = false;

  while (s21_is_space(str[*str_index])) {
    (*str_index)++;
  }
  if (str[*str_index] == '-') {
    (*str_index)++;

    sign_check = true;
  }
  if (str[*str_index] == '+') {
    (*str_index)++;

    sign_check = true;
  }
  if (str[*str_index] == '0' && str[*str_index + 1] != 'x' &&
      str[*str_index + 1] != 'X') {
    it_was_is_oct = true;
    if (sign_check) {
      (*str_index)--;
    }
    s21_type_o(args, str, str_index, arg, count, check_o, size);
  } else if (str[*str_index] == '0' &&
             (str[*str_index + 1] == 'x' || str[*str_index + 1] == 'X')) {
    it_was_hex = true;
    if (sign_check) {
      (*str_index)--;
    }
    s21_type_x(args, str, str_index, arg, count, check_x, size);
  } else {
    if (sign_check) {
      (*str_index)--;
    }
    s21_type_d_and_u(args, str, str_index, arg, count, check_d, size);
  }
  if (it_was_hex && arg.width <= 1) {
    while (s21_is_hex(str[*str_index])) {
      (*str_index)++;
    }
  }
  if (it_was_is_oct) {
    while (s21_is_oct(str[*str_index])) {
      (*str_index)++;
    }
  }
}

void s21_type_p(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_p) {
  if (!arg.flag) {
    bool minus = false;
    bool check = false;
    bool check_0x = false;

    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }
    int width_remaining = arg.width;

    if (str[*str_index] == '-') {
      minus = true;
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '+') {
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '0' &&
        (str[*str_index + 1] == 'x' || str[*str_index + 1] == 'X')) {
      (*str_index)++;
      (*str_index)++;
      check_0x = true;
      if (arg.width > 0) width_remaining--;
      if (arg.width > 0) width_remaining--;
    }

    if (!s21_is_hex(str[*str_index])) {
      *check_p = true;
    }

    int long long int_result = 0;
    while (s21_is_hex(str[*str_index]) &&
           (width_remaining > 0 || arg.width == 0)) {
      check = true;
      int_result = int_result * 16 + s21_from_hex_char_to_int(str[*str_index]);
      (*str_index)++;
      if (arg.width > 0) width_remaining--;
    }
    if (minus) {
      int_result *= -1;
    }
    if (check) {
      void **argg = va_arg(args, void **);
      *argg = (void *)int_result;
    }
    if (!*check_p || check_0x) {
      (*count)++;
    }
  } else {
    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }
    if (str[*str_index] == '-') {
      (*str_index)++;
    }
    if (str[*str_index] == '+') {
      (*str_index)++;
    }
    while (s21_is_hex(str[*str_index])) {
      (*str_index)++;
    }
  }
}

void s21_type_f(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_f, bool is_L, bool is_l) {
  if (!arg.flag) {
    while (s21_is_space(str[*str_index])) {
      (*str_index)++;
    }

    int sign = 1;
    long double value = 0.0;
    int exponent_adjust = 0;
    int width_remaining = arg.width;

    if (str[*str_index] == '-') {
      sign = -1;
      (*str_index)++;
      if (arg.width == 1) *check_f = true;
      if (arg.width > 0) width_remaining--;
    }
    if (str[*str_index] == '+') {
      (*str_index)++;
      if (arg.width == 1) *check_f = true;
      if (arg.width > 0) width_remaining--;
    }

    if (!s21_is_digit(str[*str_index]) && str[*str_index] != '.') {
      *check_f = true;
    }

    if (str[*str_index] == '.' && str[*str_index + 1] != 'e' &&
        str[*str_index + 1] != 'E' && !s21_is_digit(str[*str_index + 1])) {
      *check_f = true;
    }

    if ((str[*str_index] == 'i' || str[*str_index] == 'I') &&
        (str[*str_index + 1] == 'n' || str[*str_index + 1] == 'N') &&
        (str[*str_index + 2] == 'f' || str[*str_index + 2] == 'F')) {
      *check_f = false;
      value = INFINITY;
      *str_index += 3;
    }
    if ((str[*str_index] == 'n' || str[*str_index] == 'N') &&
        (str[*str_index + 1] == 'a' || str[*str_index + 1] == 'A') &&
        (str[*str_index + 2] == 'n' || str[*str_index + 2] == 'N')) {
      *check_f = false;
      value = NAN;
      *str_index += 3;
      if (sign == -1) value = -value;
    }
    bool is_hhex = false;
    if (str[*str_index] == '0' &&
        (str[*str_index + 1] == 'x' || str[*str_index + 1] == 'X') &&
        (str[*str_index + 2]) != '\0') {
      is_hhex = true;
      *str_index += 2;
    }
    if (str[*str_index] == '0' &&
        (str[*str_index + 1] == 'x' || str[*str_index + 1] == 'X') &&
        (str[*str_index + 2]) == '\0') {
      *check_f = true;
      *str_index += 2;
    }
    if (is_hhex) {
      while (s21_is_hex(str[*str_index])) {
        value = value * 16 + s21_from_hex_char_to_int(str[*str_index]);
        (*str_index)++;
      }

      if (str[*str_index] == '.') {
        (*str_index)++;
        long double fraction = 0.0;
        int fraction_bits = 0;

        while (s21_is_hex(str[*str_index])) {
          fraction = fraction * 16 + s21_from_hex_char_to_int(str[*str_index]);
          fraction_bits++;
          (*str_index)++;
        }

        if (fraction_bits > 0) {
          value += fraction / pow(16, fraction_bits);
        }
      }

      if ((str[*str_index] == 'e' || str[*str_index] == 'E')) {
        (*str_index)++;
        int exp_sign = 1;
        if (str[*str_index] == '-') {
          exp_sign = -1;
          (*str_index)++;
        } else if (str[*str_index] == '+') {
          (*str_index)++;
        }

        int exponent = 0;
        while (s21_is_digit(str[*str_index])) {
          exponent = exponent * 10 + (str[*str_index] - '0');
          (*str_index)++;
        }

        value *= pow(16, exp_sign * exponent);
      }
    } else {
      int exponent = 0;
      while (s21_is_digit(str[*str_index]) &&
             (width_remaining > 0 || arg.width == 0)) {
        value = value * 10 + (str[*str_index] - '0');
        (*str_index)++;
        if (arg.width > 0) width_remaining--;
      }

      if (str[*str_index] == '.' && (width_remaining > 0 || arg.width == 0)) {
        (*str_index)++;
        if (arg.width > 0) width_remaining--;

        while (s21_is_digit(str[*str_index]) &&
               (width_remaining > 0 || arg.width == 0)) {
          value = value * 10 + (str[*str_index] - '0');
          exponent_adjust--;
          (*str_index)++;
          if (arg.width > 0) width_remaining--;
        }
      }

      if ((str[*str_index] == 'e' || str[*str_index] == 'E') &&
          (width_remaining > 0 || arg.width == 0)) {
        (*str_index)++;
        if (arg.width > 0) width_remaining--;

        int exp_sign = 1;
        if (str[*str_index] == '-') {
          exp_sign = -1;
          (*str_index)++;
          if (arg.width > 0) width_remaining--;
        }
        if (str[*str_index] == '+') {
          (*str_index)++;
          if (arg.width > 0) width_remaining--;
        }

        while (s21_is_digit(str[*str_index]) &&
               (width_remaining > 0 || arg.width == 0)) {
          exponent = exponent * 10 + (str[*str_index] - '0');
          (*str_index)++;
          if (arg.width > 0) width_remaining--;
        }
        exponent *= exp_sign;
      }

      exponent += exponent_adjust;
      if (exponent != 0) {
        long double multiplier = 1.0;
        int abs_exponent = exponent > 0 ? exponent : -exponent;
        while (abs_exponent > 0) {
          long double term = 10.0;
          int power = 1;
          while ((power * 2) <= abs_exponent) {
            term *= term;
            power *= 2;
          }
          multiplier *= term;
          abs_exponent -= power;
        }
        value *= (exponent > 0) ? multiplier : (1.0 / multiplier);
      }
    }
    value *= sign;

    if (!*check_f) {
      if (is_L) {
        long double *argg = va_arg(args, long double *);
        *argg = value;
      } else if (is_l) {
        double *argg = va_arg(args, double *);
        *argg = value;
      } else {
        float *argg = va_arg(args, float *);
        *argg = (float)value;
      }
    }

    if (!*check_f) {
      (*count)++;
    }
  } else {
    s21_skip(str, str_index, check_f, s21_is_digit);

    if (str[*str_index] == '.') {
      (*str_index)++;
      while (s21_is_digit(str[*str_index])) {
        (*str_index)++;
      }
    }

    if (str[*str_index] == 'e' || str[*str_index] == 'E') {
      (*str_index)++;
      if (str[*str_index] == '-') {
        (*str_index)++;
      }
      if (str[*str_index] == '+') {
        (*str_index)++;
      }
      while (s21_is_digit(str[*str_index])) {
        (*str_index)++;
      }
    }
  }
}

int s21_sscanf(const char *str, const char *format, ...) {
  int count = -1;

  bool is_empty_or_whitespace = true;
  if (str != NULL) {
    for (int i = 0; str[i] != '\0'; i++) {
      if (!s21_is_space(str[i])) {
        is_empty_or_whitespace = false;
        break;
      }
    }
  } else {
    is_empty_or_whitespace = true;
  }

  bool is_format_empty_or_whitespace = true;
  if (format != NULL) {
    for (int i = 0; format[i] != '\0'; i++) {
      if (!s21_is_space(format[i])) {
        is_format_empty_or_whitespace = false;
        break;
      }
    }
  } else {
    is_format_empty_or_whitespace = true;
  }

  if (is_format_empty_or_whitespace) {
    count = 0;
  } else if (!is_empty_or_whitespace && str != NULL && str[0] != '\0') {
    va_list args;
    va_start(args, format);

    count = 0;

    int str_index = 0;
    int format_index = 0;
    bool check_d = false;
    bool check_o = false;
    bool check_x = false;
    bool check_f = false;
    bool check_p = false;

    while (format[format_index] != '\0') {
      if (check_d || check_o || check_x || check_p || check_f) {
        break;
      }

      if (s21_is_space(format[format_index])) {
        while (s21_is_space(str[str_index])) {
          str_index++;
        }
        format_index++;
        continue;
      } else if (format[format_index] == '%') {
        Args arg = {0, 0, 0};
        format_index++;

        if (format[format_index] == '%') {
          format_index++;
          while (s21_is_space(str[str_index])) {
            str_index++;
          }
          if (str[str_index] != '%') {
            break;
          }
          str_index++;
          continue;
        }

        if (format[format_index] == '*') {
          arg.flag = 1;
          format_index++;
        }

        while (s21_is_digit(format[format_index])) {
          arg.width = arg.width * 10 + (format[format_index] - '0');
          format_index++;
        }

        arg.type = format[format_index];
        format_index++;

        switch (arg.type) {
          case 's':
            s21_type_s(args, str, &str_index, arg, &count);
            break;
          case 'n': {
            int *argg = va_arg(args, int *);
            *argg = str_index;
            break;
          }
          case 'c':
            s21_type_c(args, str, &str_index, arg, &count);
            break;
          case 'd':
          case 'u':
            s21_type_d_and_u(args, str, &str_index, arg, &count, &check_d,
                             "00");
            break;
          case 'o':
            s21_type_o(args, str, &str_index, arg, &count, &check_o, "00");
            break;
          case 'x':
          case 'X':
            s21_type_x(args, str, &str_index, arg, &count, &check_x, "00");
            break;
          case 'i':
            s21_type_i(args, str, &str_index, arg, &count, &check_x, &check_o,
                       &check_d, "00");
            break;
          case 'p':
            s21_type_p(args, str, &str_index, arg, &count, &check_p);
            break;
          case 'f':
            s21_type_f(args, str, &str_index, arg, &count, &check_f, false,
                       false);
            break;
          case 'e':
          case 'E':
          case 'g':
          case 'G':
            s21_type_f(args, str, &str_index, arg, &count, &check_f, false,
                       false);
            break;
          case 'L': {
            char next_format = format[format_index];
            format_index++;
            switch (next_format) {
              case 'f':
              case 'e':
              case 'E':
              case 'g':
              case 'G':
                s21_type_f(args, str, &str_index, arg, &count, &check_f, true,
                           false);
                break;
              case 'x':
              case 'X':
                s21_type_x(args, str, &str_index, arg, &count, &check_x, "ll");
                break;
            }
            break;
          }
          case 'h': {
            arg.type = format[format_index];
            format_index++;
            switch (arg.type) {
              case 'h': {
                arg.type = format[format_index];
                format_index++;
                switch (arg.type) {
                  case 'i':
                    s21_type_i(args, str, &str_index, arg, &count, &check_x,
                               &check_o, &check_d, "hh");
                    break;
                  case 'o':
                    s21_type_o(args, str, &str_index, arg, &count, &check_o,
                               "hh");
                    break;
                  case 'd':
                  case 'u':
                    s21_type_d_and_u(args, str, &str_index, arg, &count,
                                     &check_d, "hh");
                    break;
                  case 'x':
                  case 'X':
                    s21_type_x(args, str, &str_index, arg, &count, &check_x,
                               "hh");
                    break;
                  case 'n': {
                    unsigned char *argg = va_arg(args, unsigned char *);
                    *argg = (unsigned char)str_index;
                    break;
                  }
                }
                break;
                case 'i':
                  s21_type_i(args, str, &str_index, arg, &count, &check_x,
                             &check_o, &check_d, "h1");
                  break;
                case 'o':
                  s21_type_o(args, str, &str_index, arg, &count, &check_o,
                             "h1");
                  break;
                case 'd':
                case 'u':
                  s21_type_d_and_u(args, str, &str_index, arg, &count, &check_d,
                                   "h1");
                  break;
                case 'x':
                case 'X':
                  s21_type_x(args, str, &str_index, arg, &count, &check_x,
                             "h1");
                  break;
                case 'n': {
                  short *argg = va_arg(args, short *);
                  *argg = (short)str_index;
                  break;
                }
              }
            }
            break;
          }
          case 'l': {
            arg.type = format[format_index];
            format_index++;
            switch (arg.type) {
              case 'd':
              case 'u':
                s21_type_d_and_u(args, str, &str_index, arg, &count, &check_d,
                                 "l1");
                break;
              case 'o':
                s21_type_o(args, str, &str_index, arg, &count, &check_o, "l1");
                break;
              case 'x':
              case 'X':
                s21_type_x(args, str, &str_index, arg, &count, &check_x, "l1");
                break;
              case 'i':
                s21_type_i(args, str, &str_index, arg, &count, &check_x,
                           &check_o, &check_d, "l1");
                break;
              case 'f':
              case 'e':
              case 'E':
              case 'g':
              case 'G':
                s21_type_f(args, str, &str_index, arg, &count, &check_f, false,
                           true);
                break;
              case 'n': {
                long *argg = va_arg(args, long *);
                *argg = (long)str_index;
                break;
              }

              case 'l': {
                arg.type = format[format_index];
                format_index++;
                switch (arg.type) {
                  case 'd':
                  case 'u':
                    s21_type_d_and_u(args, str, &str_index, arg, &count,
                                     &check_d, "ll");
                    break;
                  case 'o':
                    s21_type_o(args, str, &str_index, arg, &count, &check_o,
                               "ll");
                    break;
                  case 'x':
                  case 'X':
                    s21_type_x(args, str, &str_index, arg, &count, &check_x,
                               "ll");
                    break;
                  case 'i':
                    s21_type_i(args, str, &str_index, arg, &count, &check_x,
                               &check_o, &check_d, "ll");
                    break;
                  case 'n': {
                    long long *argg = va_arg(args, long long *);
                    *argg = (long long)str_index;
                    break;
                  }
                }
                break;
              }
            }
            break;
          }
        }
      } else {
        while (s21_is_space(str[str_index])) {
          str_index++;
        }
        if (str[str_index] != format[format_index]) {
          break;
        }
        str_index++;
        format_index++;
      }
    }

    va_end(args);
  }

  if (is_empty_or_whitespace && !is_format_empty_or_whitespace) {
    count = -1;
  }

  return count;
}
