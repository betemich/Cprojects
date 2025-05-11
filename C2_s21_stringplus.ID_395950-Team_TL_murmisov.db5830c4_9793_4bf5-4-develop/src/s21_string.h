#ifndef S21_STRING_H
#define S21_STRING_H
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define s21_NULL ((void *)0)
typedef unsigned long int s21_size_t;

int s21_sscanf(const char *str, const char *format, ...);
int s21_sprintf(char *str, const char *format, ...);
void *s21_memchr(const void *str, int ch, s21_size_t n);
s21_size_t s21_strlen(const char *str);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int ch, s21_size_t n);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int ch);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strerror(int errnum);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int ch);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);

#define FLAGS (const char *)"+-#0 "
#define LEN_FLAGS (const char *)"hlL"
#define FRM_SPEC (const char *)"cdfsugGeExXop"
#define MAX_FRMT_SIZE 50
#define BUF_SIZE 1000000
#define ULL unsigned long long

typedef enum {
  TYPE_INT,
  TYPE_UNSIGNED_INT,
  TYPE_CHAR,
  TYPE_STRING,
  TYPE_DOUBLE,
  TYPE_POINTER
} Types;

typedef enum { c, d, e, E, f, g, G, o, s, u, x, X, prcnt, p } FrmSpec;

typedef struct {
  int plus, minus, grid, zero, space;
} Flags;

typedef struct {
  long long int_arg;
  ULL ull_arg;
  char char_arg;
  long double double_arg;
  char *string_arg;
  void *ptr_arg;
} TypeArgs;

typedef struct {
  int count_args;
  char (*args_list)[BUF_SIZE];

  char (*frm_args_list)[MAX_FRMT_SIZE];
  int *frm_indexes;
  int CFALS;
  int MFALS;
  FrmSpec frm_spec;
  Flags frm_flags;
  Types type;
  TypeArgs type_args;
  int width_asterisk;
  int prec_asterisk;
  int width;
  int prec;
  int l, L, h;
} Formats;

typedef struct {
  int percent;
  s21_size_t start_form;
  int frm_args;
  int prec;
} ParseSpecStruct;

int s21_parseFormats(const char *format, Formats *frmts);
int s21_addToFAL(Formats *frmts, const char *format, s21_size_t start_form,
                 s21_size_t end_form);
int s21_isDigit(char c);
int s21_checkPercent(Formats *frmts, ParseSpecStruct *pss, const char *format,
                     int i);
void s21_argProcessing(char *buf, int *buf_it, Formats *frmts, va_list *va_args,
                       int frmt_it);
void s21_setSpecs(Formats *frmts, int frmt_it);
void s21_setAsterisks(Formats *frmts, int frmt_it);
void s21_setWidth(Formats *frmts, int frmt_it, va_list *va_args);
void s21_setPrec(Formats *frmts, int frmt_it, va_list *va_args);
void s21_setLength(Formats *frmts, int frmt_it);
void s21_setFlags(Formats *frmts, int frmt_it);
void s21_setArg(Formats *frmts, va_list *va_args, void **arg);
void s21_zeroing(Formats *frmts);
int s21_max(int v1, int v2);
void s21_charToString(char c, char *str);
int s21_intToString(long long d, char *str);
int s21_digits(long long d);
int s21_conversion(char *hex_string, ULL orig_num, int base,
                   const Formats *frmts);
void s21_process_c(char *buf, int **buf_it, void *arg, const Formats *frmts);
void s21_process_d(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_e(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_f(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_g(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_s(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_u(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_ox(char *buf, int **buf_it, void *arg, Formats *frmts);
void s21_process_prcnt(char *buf, int **buf_it);
void s21_process_p(char *buf, int **buf_it, void *arg, Formats *frmts);

typedef struct {
  int flag;
  int width;
  char type;
} Args;

int s21_is_space(char c);
int s21_is_digit(char c);
int s21_is_hex(char c);
int s21_is_oct(char c);
int s21_from_hex_char_to_int(char c);

void s21_type_s(va_list args, const char *str, int *str_index, Args arg,
                int *count);
void s21_type_c(va_list args, const char *str, int *str_index, Args arg,
                int *count);
void s21_type_d_and_u(va_list args, const char *str, int *str_index, Args arg,
                      int *count, bool *check_d, const char *size);
void s21_type_o(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_o, const char *size);
void s21_type_x(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_x, const char *size);
void s21_type_i(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_x, bool *check_o, bool *check_d,
                const char *size);
void s21_type_p(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_p);
void s21_type_f(va_list args, const char *str, int *str_index, Args arg,
                int *count, bool *check_f, bool is_L, bool is_l);

#endif
