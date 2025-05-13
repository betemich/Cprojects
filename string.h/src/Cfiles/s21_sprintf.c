#include "../s21_string.h"

int s21_sprintf(char *str, const char *format, ...) {
  va_list va_args;
  Formats frmts;
  frmts.frm_args_list = malloc(sizeof(char[MAX_FRMT_SIZE]));
  if (frmts.frm_args_list == s21_NULL) {
    fprintf(stderr, "Memory error\n");
    return 0;
  }

  frmts.frm_indexes = malloc(sizeof(int));
  if (frmts.frm_indexes == s21_NULL) {
    fprintf(stderr, "Memory error\n");
    free(frmts.frm_args_list);
    return 0;
  }

  frmts.frm_indexes[0] = -1;

  frmts.count_args = 0;
  frmts.CFALS = 0;
  frmts.MFALS = 1;

  if (s21_parseFormats(format, &frmts) == -1) {
    free(frmts.frm_args_list);
    free(frmts.frm_indexes);
    return 0;
  }

  frmts.args_list = malloc(frmts.count_args * sizeof(char[BUF_SIZE]));

  if (frmts.args_list == s21_NULL) {
    fprintf(stderr, "Memory error\n");
    free(frmts.frm_args_list);
    free(frmts.frm_indexes);
    return 0;
  }

  va_start(va_args, format);
  int frmt_it = 0;
  int buf_it = 0;
  int format_len = (int)s21_strlen(format);

  for (int i = 0; i < format_len; i++) {
    if (frmt_it < frmts.CFALS && i == frmts.frm_indexes[frmt_it]) {
      s21_argProcessing(str, &buf_it, &frmts, &va_args, frmt_it);
      i += s21_strlen(frmts.frm_args_list[frmt_it++]) - 1;
    } else {
      str[buf_it++] = format[i];
    }
  }

  str[buf_it] = '\0';
  free(frmts.frm_args_list);
  free(frmts.args_list);
  free(frmts.frm_indexes);
  va_end(va_args);
  return buf_it;
}

int s21_max(int v1, int v2) { return v1 > v2 ? v1 : v2; }

int s21_min(int v1, int v2) { return v1 > v2 ? v2 : v1; }

void s21_argProcessing(char *buf, int *buf_it, Formats *frmts, va_list *va_args,
                       int frmt_it) {
  void *arg = s21_NULL;
  s21_zeroing(frmts);
  s21_setSpecs(frmts, frmt_it);
  s21_setFlags(frmts, frmt_it);
  s21_setAsterisks(frmts, frmt_it);
  s21_setLength(frmts, frmt_it);
  s21_setArg(frmts, va_args, &arg);
  s21_setWidth(frmts, frmt_it, va_args);
  s21_setPrec(frmts, frmt_it, va_args);
  switch (frmts->frm_spec) {
    case c:
      s21_process_c(buf, &buf_it, arg, frmts);
      break;
    case d:
      s21_process_d(buf, &buf_it, arg, frmts);
      break;
    case E:
    case e:
      s21_process_e(buf, &buf_it, arg, frmts);
      break;
    case f:
      s21_process_f(buf, &buf_it, arg, frmts);
      break;
    case G:
    case g:
      s21_process_g(buf, &buf_it, arg, frmts);
      break;
    case s:
      s21_process_s(buf, &buf_it, arg, frmts);
      break;
    case u:
      s21_process_u(buf, &buf_it, arg, frmts);
      break;
    case o:
    case x:
    case X:
      s21_process_ox(buf, &buf_it, arg, frmts);
      break;
    case prcnt:
      s21_process_prcnt(buf, &buf_it);
      break;
    case p:
      s21_process_p(buf, &buf_it, arg, frmts);
      break;
  }
}

int s21_digits(long long d) {
  if (d == 0) {
    return 1;
  }
  int dig = 0;
  if (d < 0) {
    d = -d;
  }
  while (d != 0) {
    d /= 10;
    dig++;
  }
  return dig;
}

int s21_unsignedDigits(unsigned long long d) {
  if (d == 0) {
    return 1;
  }
  int dig = 0;
  while (d != 0) {
    d /= 10;
    dig++;
  }
  return dig;
}

int s21_intToString(long long d, char *str) {
  if (d < 0) {
    d = -d;
  }
  int dig = s21_digits(d);
  for (int i = 0; i < dig; i++) {
    str[i] = (char)(d % 10 + '0');
    d /= 10;
  }

  for (int i = 0; i < dig / 2; i++) {
    char temp = str[i];
    str[i] = str[dig - 1 - i];
    str[dig - 1 - i] = temp;
  }
  str[dig] = '\0';
  return dig;
}

void s21_unsignedToString(unsigned long long d, char *str) {
  int dig = s21_unsignedDigits(d);
  for (int i = 0; i < dig; i++) {
    str[i] = (char)(d % 10 + '0');
    d /= 10;
  }

  for (int i = 0; i < dig / 2; i++) {
    char temp = str[i];
    str[i] = str[dig - 1 - i];
    str[dig - 1 - i] = temp;
  }
  str[dig] = '\0';
}

void s21_checkSign(long double *orig_double, char *string_double, int *i,
                   const Formats *frmts) {
  if (*orig_double < 0) {
    string_double[(*i)++] = '-';
    *orig_double = -*orig_double;
  } else if (frmts->frm_flags.space) {
    string_double[(*i)++] = ' ';
  } else if (frmts->frm_flags.plus) {
    string_double[(*i)++] = '+';
  }
}

int s21_calcDeg(long double *orig_double, const Formats *frmts) {
  int deg = 0;
  long double error = 0.0L;
  if (frmts->L == 0)
    error = 1e-16;
  else
    error = 1e-20;

  if (*orig_double <= 0.0L && *orig_double + error >= 0.0L) {
    return 0;
  }

  while (*orig_double + error < 1.0L) {
    *orig_double *= 10.0L;
    deg--;
  }
  while (*orig_double + error >= 10.0L) {
    *orig_double /= 10.0L;
    deg++;
  }
  return deg;
}

void s21_makeDoubleBase(const long double *orig_double, char *string_double,
                        int *i, const Formats *frmts) {
  long double whole_double;
  long double fractional = modfl(*orig_double, &whole_double);
  long long int whole_int = (long long int)whole_double;
  char string_whole[100];
  s21_intToString(whole_int, string_whole);
  int j = 0;
  while (j < (int)s21_strlen(string_whole)) {
    string_double[(*i)++] = string_whole[j++];
  }

  if (frmts->prec != 0 || frmts->frm_flags.grid == 1) {
    string_double[(*i)++] = '.';
  }

  int count = (frmts->prec == -1) ? 6 : frmts->prec;
  for (int c = 0; c < count; c++) {
    fractional *= 10.0L;
    int carry = (int)(fractional);
    string_double[(*i)++] = (char)((carry) + '0');
    fractional -= carry;
  }
  if ((int)(fractional * 10.0L) >= 5) {
    j = *i - 1;
    while (j != -1 && string_double[j] == '9') {
      string_double[j] = '0';
      j--;
    }
    if (j == -1 || (j == 0 && s21_strchr("+- ", string_double[0]))) {
      j = 0;
      *i += 1;
      if (s21_strchr("+- ", string_double[0])) {
        string_double[j++] = string_double[0];
      }
      string_double[j++] = '1';
      for (int c = 1; c <= s21_digits(whole_int); c++) {
        string_double[j++] = '0';
      }
    } else if (string_double[j] == '.') {
      j--;
      while (j != -1 && string_double[j] == '9') {
        string_double[j] = '0';
        j--;
      }
      if (j == -1 || (j == 0 && s21_strchr("+- ", string_double[0]))) {
        *i += 1;
        j = 0;
        if (s21_strchr("+- ", string_double[0])) {
          string_double[j++] = string_double[0];
        }
        string_double[j++] = '1';
        for (int c = 1; c <= s21_digits(whole_int); c++) {
          string_double[j++] = '0';
        }
        if (frmts->prec != 0 || frmts->frm_flags.grid == 1)
          string_double[j++] = '.';
        for (int c = 0; c < count; c++) {
          string_double[j++] = '0';
        }
      } else {
        string_double[j]++;
      }
    } else {
      string_double[j]++;
    }
  }
}

int s21_conversion(char *res_string, ULL orig_num, int base,
                   const Formats *frmts) {
  int i = 0;
  if (orig_num == 0) {
    res_string[i++] = '0';
    res_string[i] = '\0';
    return i;
  }
  while (orig_num != 0) {
    ULL res = orig_num % base;
    switch (res) {
      case 10:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'a' : 'A';
        break;
      case 11:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'b' : 'B';
        break;
      case 12:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'c' : 'C';
        break;
      case 13:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'd' : 'D';
        break;
      case 14:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'e' : 'E';
        break;
      case 15:
        res_string[i++] =
            frmts->frm_spec == x || frmts->frm_spec == p ? 'f' : 'F';
        break;
      default:
        res_string[i++] = (char)(res + '0');
        break;
    }
    orig_num /= base;
  }
  for (int j = 0; j < i / 2; j++) {
    char temp = res_string[j];
    res_string[j] = res_string[i - j - 1];
    res_string[i - j - 1] = temp;
  }
  res_string[i] = '\0';
  return i;
}

void s21_bufferInt(char *buf, int ***buf_it, const char *res, int len,
                   Formats *frmts) {
  int sign = 0;
  int isSign = s21_strchr("+- ", res[0]) ? 1 : 0;
  if (frmts->frm_flags.minus == 0) {
    if (frmts->frm_flags.zero == 1 && frmts->prec == -1 &&
        frmts->width - s21_max(frmts->prec, len) > 0 && isSign) {
      buf[(***buf_it)++] = res[0];
      sign = 1;
    }
    for (int i = 0; i < frmts->width - s21_max(frmts->prec + isSign, len);
         i++) {
      buf[(***buf_it)++] =
          frmts->frm_flags.zero == 1 && frmts->prec == -1 ? '0' : ' ';
    }
    if (frmts->prec - len >= 0 && isSign) {
      buf[(***buf_it)++] = res[0];
      sign = 1;
    }
    for (int i = 0; i < frmts->prec - len + sign; i++) {
      buf[(***buf_it)++] = '0';
    }
    for (int i = sign; i < len; i++) {
      buf[(***buf_it)++] = res[i];
    }
  } else {
    if (frmts->prec - len >= 0 && isSign) {
      buf[(***buf_it)++] = res[0];
      sign = 1;
    }
    for (int i = 0; i < frmts->prec - len + sign; i++) {
      buf[(***buf_it)++] = '0';
    }
    for (int i = sign; i < len; i++) {
      buf[(***buf_it)++] = res[i];
    }
    for (int i = 0; i < frmts->width - s21_max(frmts->prec, len); i++) {
      buf[(***buf_it)++] = ' ';
    }
  }
}

void s21_bufferDefault(char *buf, int ***buf_it, const char *res, int len,
                       Formats *frmts) {
  if (frmts->frm_flags.minus == 0) {
    for (int j = 0; j < frmts->width - len; j++) {
      buf[(***buf_it)++] =
          frmts->frm_flags.zero == 1 && frmts->frm_spec != s ? '0' : ' ';
    }
    for (int j = 0; j < len; j++) {
      buf[(***buf_it)++] = res[j];
    }
  } else {
    for (int j = 0; j < len; j++) {
      buf[(***buf_it)++] = res[j];
    }
    for (int j = 0; j < frmts->width - len; j++) {
      buf[(***buf_it)++] = ' ';
    }
  }
}

void s21_setArg(Formats *frmts, va_list *va_args, void **arg) {
  Types type = frmts->type;
  switch (type) {
    case TYPE_INT:
      if (frmts->h == 2) {
        char c = va_arg(*va_args, int);
        frmts->type_args.int_arg = (long long)c;
      } else if (frmts->h == 1) {
        short c = va_arg(*va_args, int);
        frmts->type_args.int_arg = (long long)c;
      } else if (frmts->l == 0) {
        int c = va_arg(*va_args, int);
        frmts->type_args.int_arg = (long long)c;
      } else {
        frmts->type_args.int_arg = va_arg(*va_args, long long);
      }
      *arg = (void *)(intptr_t)&frmts->type_args.int_arg;
      break;
    case TYPE_UNSIGNED_INT:
      if (frmts->h == 2) {
        unsigned char c = va_arg(*va_args, int);
        frmts->type_args.ull_arg = (ULL)c;
      } else if (frmts->h == 1) {
        unsigned short c = va_arg(*va_args, int);
        frmts->type_args.ull_arg = (ULL)c;
      } else if (frmts->l == 0) {
        unsigned int c = va_arg(*va_args, unsigned int);
        frmts->type_args.ull_arg = (ULL)c;
      } else {
        frmts->type_args.ull_arg = va_arg(*va_args, unsigned long long);
      }
      *arg = (void *)(intptr_t)&frmts->type_args.ull_arg;
      break;
    case TYPE_CHAR:
      frmts->type_args.char_arg = (char)va_arg(*va_args, int);
      *arg = (void *)(intptr_t)&frmts->type_args.char_arg;
      break;
    case TYPE_STRING:
      frmts->type_args.string_arg = va_arg(*va_args, char *);
      *arg = (void *)(intptr_t)frmts->type_args.string_arg;
      break;
    case TYPE_DOUBLE:
      if (frmts->L > 0) {
        frmts->type_args.double_arg = va_arg(*va_args, long double);
      } else {
        frmts->type_args.double_arg = va_arg(*va_args, double);
      }
      *arg = (void *)(intptr_t)&frmts->type_args.double_arg;
      break;
    case TYPE_POINTER:
      frmts->type_args.ptr_arg = va_arg(*va_args, void *);
      *arg = frmts->type_args.ptr_arg;
      break;
  }
}

void s21_setSpecs(Formats *frmts, int frmt_it) {
  s21_size_t len = s21_strlen(frmts->frm_args_list[frmt_it]);
  switch (frmts->frm_args_list[frmt_it][len - 1]) {
    case 'd':
      frmts->frm_spec = d;
      frmts->type = TYPE_INT;
      break;
    case 'c':
      frmts->frm_spec = c;
      frmts->type = TYPE_CHAR;
      break;
    case 'e':
      frmts->frm_spec = e;
      frmts->type = TYPE_DOUBLE;
      break;
    case 'E':
      frmts->frm_spec = E;
      frmts->type = TYPE_DOUBLE;
      break;
    case 'f':
      frmts->frm_spec = f;
      frmts->type = TYPE_DOUBLE;
      break;
    case 'g':
      frmts->frm_spec = g;
      frmts->type = TYPE_DOUBLE;
      break;
    case 'G':
      frmts->frm_spec = G;
      frmts->type = TYPE_DOUBLE;
      break;
    case 'o':
      frmts->frm_spec = o;
      frmts->type = TYPE_UNSIGNED_INT;
      break;
    case 's':
      frmts->frm_spec = s;
      frmts->type = TYPE_STRING;
      break;
    case 'u':
      frmts->frm_spec = u;
      frmts->type = TYPE_UNSIGNED_INT;
      break;
    case 'x':
      frmts->frm_spec = x;
      frmts->type = TYPE_UNSIGNED_INT;
      break;
    case 'X':
      frmts->frm_spec = X;
      frmts->type = TYPE_UNSIGNED_INT;
      break;
    case '%':
      frmts->frm_spec = prcnt;
      break;
    case 'p':
      frmts->frm_spec = p;
      frmts->type = TYPE_POINTER;
      break;
  }
}

void s21_setFlags(Formats *frmts, int frmt_it) {
  const char *frmt_str = frmts->frm_args_list[frmt_it];
  int it = 1;
  while (s21_strchr(FLAGS, frmt_str[it]) != s21_NULL) {
    switch (frmt_str[it++]) {
      case '+':
        frmts->frm_flags.plus += 1;
        break;
      case '-':
        frmts->frm_flags.minus += 1;
        break;
      case '#':
        frmts->frm_flags.grid += 1;
        break;
      case '0':
        frmts->frm_flags.zero += 1;
        break;
      case ' ':
        frmts->frm_flags.space += 1;
        break;
    }
  }
}

void s21_setAsterisks(Formats *frmts, int frmt_it) {
  const char *frmt_str = frmts->frm_args_list[frmt_it];
  const char *str_after_strchr = s21_strchr(frmt_str, '*');
  const char *point_str = s21_strchr(frmt_str, '.');
  if (point_str != s21_NULL && s21_strchr(point_str, '*') != s21_NULL) {
    frmts->prec_asterisk = 1;
  }
  if (str_after_strchr != s21_NULL &&
      str_after_strchr != s21_strchr(point_str, '*')) {
    frmts->width_asterisk = 1;
  }
}

void s21_setWidth(Formats *frmts, int frmt_it, va_list *va_args) {
  if (frmts->width_asterisk) {
    frmts->width = va_arg(*va_args, int);
    return;
  }
  const char *frmt_str = frmts->frm_args_list[frmt_it];
  int it = 1;
  while (s21_strchr(FLAGS, frmt_str[it]) != s21_NULL) it++;
  while (s21_isDigit(frmt_str[it])) {
    if (frmts->width == -1) {
      frmts->width = 0;
    }
    frmts->width = frmts->width * 10 + (int)frmt_str[it++] - '0';
  }
}

void s21_setPrec(Formats *frmts, int frmt_it, va_list *va_args) {
  if (frmts->prec_asterisk) {
    frmts->prec = va_arg(*va_args, int);
    return;
  }
  const char *point_str = s21_strchr(frmts->frm_args_list[frmt_it], '.');
  if (point_str == s21_NULL) {
    return;
  }
  int it = 1;
  frmts->prec = 0;
  while (s21_isDigit(point_str[it])) {
    frmts->prec = frmts->prec * 10 + (int)point_str[it++] - '0';
  }
}

void s21_setLength(Formats *frmts, int frmt_it) {
  const char *frmt_str = frmts->frm_args_list[frmt_it];
  s21_size_t len = s21_strlen(frmt_str);
  s21_size_t it = 0;
  while (it < len && s21_strchr(LEN_FLAGS, frmt_str[it]) == s21_NULL) it++;
  if (it == len) {
    return;
  }
  while (s21_strchr(LEN_FLAGS, frmt_str[it]) != s21_NULL) {
    switch (frmt_str[it++]) {
      case 'l':
        frmts->l++;
        break;
      case 'L':
        frmts->L++;
        break;
      case 'h':
        frmts->h++;
        break;
    }
  }
}

void s21_zeroing(Formats *frmts) {
  frmts->width_asterisk = 0;
  frmts->prec_asterisk = 0;
  frmts->width = -1;
  frmts->prec = -1;
  frmts->l = 0;
  frmts->L = 0;
  frmts->h = 0;
  s21_memset(&frmts->frm_flags, 0, sizeof(frmts->frm_flags));
  s21_memset(&frmts->type_args, 0, sizeof(frmts->type_args));
  frmts->type = -1;
  frmts->frm_spec = 0;
}

void s21_process_c(char *buf, int **buf_it, void *arg, const Formats *frmts) {
  char char_arg = *(char *)arg;
  if (frmts->frm_flags.minus == 0) {
    for (int i = 0; i < frmts->width - 1; i++) {
      buf[(**buf_it)++] = ' ';
    }
    buf[(**buf_it)++] = char_arg;
  } else {
    buf[(**buf_it)++] = char_arg;
    for (int i = 1; i < frmts->width; i++) {
      buf[(**buf_it)++] = ' ';
    }
  }
}

void s21_process_d(char *buf, int **buf_it, void *arg, Formats *frmts) {
  long long orig_int = *(long long *)arg;

  if (orig_int == 0 && frmts->prec == 0) {
    for (int i = 0; i < frmts->width; i++) {
      buf[(**buf_it)++] = ' ';
    }
    return;
  }

  char res_string[1000];
  int len = 0;
  if (orig_int < 0) {
    res_string[len++] = '-';
  } else if (frmts->frm_flags.space) {
    res_string[len++] = ' ';
  } else if (frmts->frm_flags.plus) {
    res_string[len++] = '+';
  }

  char str_int[1000];
  int len_str_int = s21_intToString(orig_int, str_int);
  for (int i = 0; i < len_str_int; i++) {
    res_string[len++] = str_int[i];
  }
  res_string[len] = '\0';
  s21_bufferInt(buf, &buf_it, res_string, len, frmts);
}

void s21_process_e(char *buf, int **buf_it, void *arg, Formats *frmts) {
  long double orig_double = *(long double *)arg;
  char string_double[500];
  int i = 0;
  s21_checkSign(&orig_double, string_double, &i, frmts);

  int deg = s21_calcDeg(&orig_double, frmts);

  s21_makeDoubleBase(&orig_double, string_double, &i, frmts);
  string_double[i++] = frmts->frm_spec == e ? 'e' : 'E';
  string_double[i++] = deg >= 0 ? '+' : '-';
  if (deg > -10 && deg < 10) {
    string_double[i++] = '0';
  }
  char deg_string[100];
  s21_intToString(deg, deg_string);
  for (s21_size_t j = 0; j < s21_strlen(deg_string); j++) {
    string_double[i++] = deg_string[j];
  }
  string_double[i] = '\0';
  s21_bufferDefault(buf, &buf_it, string_double, i, frmts);
}

void s21_process_f(char *buf, int **buf_it, void *arg, Formats *frmts) {
  long double orig_double = *(long double *)arg;
  char string_double[500];
  int i = 0;
  s21_checkSign(&orig_double, string_double, &i, frmts);
  s21_makeDoubleBase(&orig_double, string_double, &i, frmts);
  string_double[i] = '\0';
  s21_bufferDefault(buf, &buf_it, string_double, i, frmts);
}

void s21_process_g(char *buf, int **buf_it, void *arg, Formats *frmts) {
  long double orig_double = *(long double *)arg;
  char string_double[500];
  int i = 0;

  s21_checkSign(&orig_double, string_double, &i, frmts);
  long double e_double = orig_double;
  int deg = s21_calcDeg(&e_double, frmts);

  if (frmts->prec == -1) {
    frmts->prec = 6;
  }

  if (deg < -4 || (deg >= frmts->prec && deg != 0)) {
    long double whole_double;
    modfl(e_double, &whole_double);
    long long int whole_int = (long long int)whole_double;
    frmts->prec = frmts->prec - s21_digits(whole_int) >= 0
                      ? frmts->prec - s21_digits(whole_int)
                      : 0;
    s21_makeDoubleBase(&e_double, string_double, &i, frmts);
    if (frmts->frm_flags.grid == 0) {
      while (string_double[i - 1] == '0') {
        i--;
      }
      if (string_double[i - 1] == '.') {
        i--;
      }
    }
    string_double[i++] = frmts->frm_spec == g ? 'e' : 'E';
    string_double[i++] = deg > 0 ? '+' : '-';
    if (deg < 10 && deg > -10) {
      string_double[i++] = '0';
    }

    char deg_string[100];
    s21_intToString(deg, deg_string);
    for (s21_size_t c = 0; c < s21_strlen(deg_string); c++) {
      string_double[i++] = deg_string[c];
    }
  } else {
    long double whole_double;
    modfl(orig_double, &whole_double);
    long long int whole_int = (long long int)whole_double;
    frmts->prec = frmts->prec - s21_digits(whole_int) >= 0
                      ? frmts->prec - s21_digits(whole_int)
                      : 0;
    if (deg < 0) {
      deg = -deg;
      frmts->prec += deg;
    }

    s21_makeDoubleBase(&orig_double, string_double, &i, frmts);
    if (frmts->frm_flags.grid == 0) {
      while (string_double[i - 1] == '0') {
        i--;
      }
      if (string_double[i - 1] == '.') {
        i--;
      }
    }
  }
  string_double[i] = '\0';
  s21_bufferDefault(buf, &buf_it, string_double, i, frmts);
}

void s21_process_s(char *buf, int **buf_it, void *arg, Formats *frmts) {
  const char *string = (char *)arg;
  int len = s21_min(s21_strlen(string), frmts->prec);
  if (len == -1) len = s21_strlen(string);
  s21_bufferDefault(buf, &buf_it, string, len, frmts);
}

void s21_process_u(char *buf, int **buf_it, void *arg, Formats *frmts) {
  ULL orig_unsigned = *(ULL *)arg;

  if (orig_unsigned == 0 && frmts->prec == 0) {
    for (int i = 0; i < frmts->width; i++) {
      buf[(**buf_it)++] = ' ';
    }
    return;
  }

  int len = s21_unsignedDigits(orig_unsigned);
  char str_unsigned[len + 2];
  s21_unsignedToString(orig_unsigned, str_unsigned);
  s21_bufferInt(buf, &buf_it, str_unsigned, len, frmts);
}

void s21_process_ox(char *buf, int **buf_it, void *arg, Formats *frmts) {
  ULL orig_num = *(ULL *)arg;

  if (orig_num == 0 && frmts->prec == 0) {
    for (int i = 0; i < frmts->width; i++) {
      buf[(**buf_it)++] = ' ';
    }
    return;
  }

  char res_string[100];
  char num[100];
  int len_res = 0;
  if (frmts->frm_flags.grid == 1 && orig_num != 0) {
    res_string[len_res++] = '0';
    if (frmts->frm_spec == x) {
      res_string[len_res++] = 'x';
    } else if (frmts->frm_spec == X) {
      res_string[len_res++] = 'X';
    }
  }

  int base = frmts->frm_spec == o ? 8 : 16;
  int len_num = s21_conversion(num, orig_num, base, frmts);
  for (int i = 0; i < len_num; i++) {
    res_string[len_res++] = num[i];
  }

  res_string[len_res] = '\0';

  s21_bufferInt(buf, &buf_it, res_string, len_res, frmts);
}

void s21_process_p(char *buf, int **buf_it, void *arg, Formats *frmts) {
  ULL ptr = (ULL)arg;
  char ptr_string[500];
  char carry[500];
  int len_res = 0;

  ptr_string[len_res++] = '0';
  ptr_string[len_res++] = 'x';

  int len_carry = s21_conversion(carry, ptr, 16, frmts);
  for (int i = 0; i < len_carry; i++) {
    ptr_string[len_res++] = carry[i];
  }
  ptr_string[len_res] = '\0';

  s21_bufferDefault(buf, &buf_it, ptr_string, len_res, frmts);
}

void s21_process_prcnt(char *buf, int **buf_it) { buf[(**buf_it)++] = '%'; }

int s21_checkPercent(Formats *frmts, ParseSpecStruct *pss, const char *format,
                     int i) {
  int res = 0;
  if (format[i] == '%') {
    if (pss->percent) {
      if (s21_addToFAL(frmts, format, pss->start_form, i) == -1) {
        return -1;
      }
      frmts->count_args++;
      pss->percent = 0;

    } else {
      pss->percent = 1;
      pss->start_form = i;
      pss->frm_args = 0;
      pss->prec = 0;
    }
    res = 1;
  }
  return res;
}

int s21_parseFormats(const char *format, Formats *frmts) {
  ParseSpecStruct pss;
  pss.percent = 0;
  pss.start_form = 0;
  pss.prec = 0;
  pss.frm_args = 0;
  s21_size_t frmt_len = s21_strlen(format);
  for (s21_size_t i = 0; i < frmt_len; i++) {
    int carry = s21_checkPercent(frmts, &pss, format, i);
    if (carry) {
    } else if (pss.percent && s21_strchr(FLAGS, format[i]) != s21_NULL &&
               pss.frm_args == 0) {
      if (pss.frm_args != 0 && format[i] != '0') {
        pss.percent = 0;
      }
    } else if (pss.percent && (s21_isDigit(format[i]) || format[i] == '*') &&
               !pss.prec) {
      if (pss.frm_args > 1 || (format[i] == '*' && pss.frm_args == 1) ||
          format[i - 1] == '*') {
        pss.percent = 0;
      }
      if (format[i] == '*') {
        frmts->count_args++;
      }
      pss.frm_args = 1;
    } else if (pss.percent && format[i] == '.') {
      if (pss.frm_args > 1) {
        pss.percent = 0;
      }
      pss.prec = 1;
    } else if (pss.percent && (s21_isDigit(format[i]) || format[i] == '*')) {
      if (pss.frm_args > 2 || (format[i] == '*' && pss.frm_args == 2) ||
          format[i - 1] == '*') {
        pss.percent = 0;
      }
      if (format[i] == '*') {
        frmts->count_args++;
      }
      pss.frm_args = 2;
    } else if (pss.percent && s21_strchr(LEN_FLAGS, format[i]) != s21_NULL) {
      if (format[i] == 'l' && format[i - 1] == 'l') {
        continue;
      }
      if (format[i] == 'h' && format[i - 1] == 'h') {
        continue;
      }
      if (pss.frm_args >= 3) {
        pss.percent = 0;
      }
      pss.frm_args = 3;
    } else if (pss.percent && s21_strchr(FRM_SPEC, format[i])) {
      if (s21_addToFAL(frmts, format, pss.start_form, i) == -1) {
        return -1;
      }
      frmts->count_args++;
      pss.percent = 0;
    } else if (pss.percent) {
      pss.percent = 0;
    }
  }
  if (pss.percent) {
    pss.percent = 0;
  }
  return 0;
}

int s21_addToFAL(Formats *frmts, const char *format, s21_size_t start_form,
                 s21_size_t end_form) {
  if (frmts->CFALS == frmts->MFALS) {
    frmts->frm_args_list = realloc(
        frmts->frm_args_list, sizeof(char[MAX_FRMT_SIZE]) * (frmts->MFALS + 1));
    if (frmts->frm_args_list == s21_NULL) {
      fprintf(stderr, "Memory error\n");
      return -1;
    }
    frmts->frm_indexes =
        realloc(frmts->frm_indexes, sizeof(int) * (frmts->MFALS + 1));
    if (frmts->frm_indexes == s21_NULL) {
      fprintf(stderr, "Memory error\n");
      return -1;
    }
    frmts->MFALS++;
  }
  frmts->frm_indexes[frmts->CFALS] = start_form;
  s21_size_t i = start_form;
  while (i <= end_form) {
    frmts->frm_args_list[frmts->CFALS][i - start_form] = format[i];
    i++;
  }
  frmts->frm_args_list[frmts->CFALS][i - start_form] = '\0';
  frmts->CFALS++;
  return 1;
}

int s21_isDigit(char c) { return c <= '9' && c >= '0'; }