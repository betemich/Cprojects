#include "s21_grep.h"

int getRegFromFile(Arguments *args, const char *file_path) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    perror("Error");
    return -1;
  }
  char line[BUF_SIZE];
  while (fgets(line, BUF_SIZE, file)) {
    addPattern(args, line);
  }
  fclose(file);
  return 1;
}

void printMatch(const char *line, regex_t *re, const Arguments *args,
                const char *file_path, int number_line) {
  regmatch_t match;
  int offset = 0;
  while (1) {
    int result = regexec(re, line + offset, 1, &match, 0);
    if (offset >= (int)strlen(line) || result != 0) {
      break;
    }
    if (match.rm_so == match.rm_eo) {
      offset += 1;
      continue;
    }

    if (args->print_file_name && !args->h) printf("%s:", file_path);
    if (args->n) printf("%d:", number_line);
    for (int i = offset + match.rm_so; i < offset + match.rm_eo; i++) {
      putchar(line[i]);
    }
    offset += match.rm_eo;
    putchar('\n');
  }
}

void printLine(const Arguments *args, const char *line, const char *file_path,
               int number_line) {
  if (args->print_file_name && !args->h) printf("%s:", file_path);
  if (args->n) printf("%d:", number_line);
  printf("%s", line);
  if (line[strlen(line) - 1] != '\n') {
    printf("\n");
  }
}

int expandPattern(Arguments *args, int size) {
  int error = 0;
  args->pattern = (char *)realloc(args->pattern, size * sizeof(char));
  if (args->pattern == NULL) {
    perror("Error\n");
    free(args->pattern);
    error = 1;
  }
  args->max_len_pattern = size;
  return error;
}

int addPattern(Arguments *args, char *pattern) {
  int error = 0;
  if (pattern == NULL) {
    return 1;
  }
  int pattern_len = strlen(pattern);
  int expected_length = args->curr_len_pattern + pattern_len + 1;
  if (expected_length >= args->max_len_pattern) {
    error = expandPattern(args, expected_length + 100);
    if (error) {
      return 0;
    }
  }

  if (args->curr_len_pattern != 0) {
    strcat(args->pattern + args->curr_len_pattern, "|");
    args->curr_len_pattern++;
  }
  if (pattern[pattern_len - 1] == '\n') {
    pattern[pattern_len - 1] = '\0';
  }
  args->curr_len_pattern +=
      sprintf(args->pattern + args->curr_len_pattern, "(%s)", pattern);
  return 1;
}

void fileProcess(const char *file_path, const Arguments *args, regex_t re) {
  FILE *file = fopen(file_path, "r");
  if (file == NULL) {
    if (!args->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", file_path);
    return;
  }

  char line[BUF_SIZE] = "";
  int count = 0;
  int number_line = 0;

  while (fgets(line, BUF_SIZE, file)) {
    int reg = regexec(&re, line, 0, NULL, 0);
    number_line++;
    if ((!reg && !args->v) || (reg && args->v)) {
      count++;
      if (args->l) {
        printf("%s\n", file_path);
        break;
      }
      if (!args->c && !args->l && !args->o) {
        printLine(args, line, file_path, number_line);
      } else if (!args->c && !args->l && args->o) {
        printMatch(line, &re, args, file_path, number_line);
      }
    }
  }
  if (args->c && !args->l) {
    if (args->print_file_name && !args->h)
      printf("%s:%d\n", file_path, count);
    else
      printf("%d\n", count);
  }

  fclose(file);
}

int parseArguments(int argc, char **argv, Arguments *args) {
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        args->e = 1;
        if (!addPattern(args, optarg)) {
          return -1;
        }
        break;
      case 'i':
        args->i = REG_ICASE;
        break;
      case 'v':
        args->v = 1;
        break;
      case 'c':
        args->c = 1;
        break;
      case 'l':
        args->l = 1;
        break;
      case 'n':
        args->n = 1;
        break;
      case 'h':
        args->h = 1;
        break;
      case 's':
        args->s = 1;
        break;
      case 'f':
        args->f = 1;
        if (!getRegFromFile(args, optarg)) {
          return -1;
        }
        break;
      case 'o':
        args->o = 1;
        break;
      case '?':
        fprintf(stderr,
                "Usage: grep [OPTION]... PATTERNS[FILE]...\nTry \'grep "
                "--help\' for more information.\n");
        return -1;
      default:
        fprintf(stderr, "Unknown error");
        return -1;
    }
  }
  if (args->curr_len_pattern == 0) {
    addPattern(args, argv[optind++]);
  }

  return optind;
}

int main(int argc, char **argv) {
  Arguments args = {0};

  args.pattern = (char *)malloc(sizeof(char));
  if (args.pattern == NULL) {
    perror("Error\n");
    return -1;
  }
  args.pattern[0] = '\0';
  int first_file = parseArguments(argc, argv, &args);
  if (first_file == -1) {
    free(args.pattern);
    return -1;
  }

  args.print_file_name = argc - first_file <= 1 ? 0 : 1;

  regex_t re;

  int error = regcomp(&re, args.pattern, REG_EXTENDED | REG_NEWLINE | args.i);
  if (error) {
    perror("Error\n");
    free(args.pattern);
    return -1;
  }

  for (int i = first_file; i < argc; i++) {
    fileProcess(argv[i], &args, re);
  }
  free(args.pattern);
  regfree(&re);
}
