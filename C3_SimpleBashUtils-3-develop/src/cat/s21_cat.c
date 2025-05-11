#include "s21_cat.h"

void processFile(const char *file_path, Arguments *args, int first) {
  FILE *file = fopen(file_path, "r");
  if (file != NULL && !first && !args->slashN) {
    if (args->e) {
      printf("$");
    }
    printf("\n");
  }
  if (file == NULL) {
    fprintf(stderr, "cat: %s: No such file or directory\n", file_path);
    return;
  }

  unsigned char c;
  int empty = 0;
  int new_file = 1;
  while (fread(&c, 1, 1, file) > 0) {
    if (!checkS(c, &empty, new_file, args)) {
      continue;
    }

    if (c == '\n' && !first && new_file && !args->slashN) {
      continue;
    }

    if (checkB(c, &new_file, args))
      printNumber(args);
    else if (!args->b && checkN(&new_file, args))
      printNumber(args);

    if (c == '\n') {
      if (args->e)
        printf("$\n");
      else
        printf("\n");
      args->slashN = 1;

    } else {
      if (args->t && c == '\t')
        printf(TAB);
      else if (args->v && c != '\t')
        printV(c);
      else
        defaultPrint(c);

      args->slashN = 0;
      empty = 0;
      new_file = 0;
    }
  }
  fclose(file);
}

void printNumber(Arguments *args) { printf("%6d\t", (args->number)++); }

void defaultPrint(unsigned char c) { printf("%c", c); }

void printV(unsigned char c) {
  if (c < 32)
    printf("^%c", c + 64);
  else if (c == 127)
    printf("^?");
  else if (c > 127 && c < 160)
    printf("M-^%c", c - 64);
  else if (c > 160)
    printf("M-%c", c - 128);
  else
    printf("%c", c);
}

int checkS(unsigned char c, int *empty, int new_file, const Arguments *args) {
  int res = 1;
  if (args->s) {
    if (c == '\n' && (args->slashN || new_file)) {
      if (*empty)
        res = 0;
      else
        *empty = 1;
    } else if (c != '\n' && *empty)
      *empty = 0;
  }
  return res;
}

int checkN(const int *new_file, const Arguments *args) {
  int res = 0;
  if ((args->slashN || *new_file) && args->n) res = 1;
  return res;
}

int checkB(unsigned char c, const int *new_file, const Arguments *args) {
  int res = 0;
  if (c != '\n' && (args->slashN || *new_file) && args->b) res = 1;
  return res;
}

const struct option long_options[] = {{"number-nonblank", no_argument, 0, 'b'},
                                      {"number", no_argument, 0, 'n'},
                                      {"squeeze-blank", no_argument, 0, 's'},
                                      {0, 0, 0, 0}};

int parseArguments(int argc, char **argv, Arguments *args) {
  int opt;
  int long_opt_ind = -1;
  while ((opt = getopt_long(argc, argv, "benstvET", long_options,
                            &long_opt_ind)) != -1) {
    switch (opt) {
      case 'b':
        args->b = 1;
        break;
      case 'e':
        args->e = 1;
        args->v = 1;
        break;
      case 'E':
        args->e = 1;
        break;
      case 'n':
        args->n = 1;
        break;
      case 's':
        args->s = 1;
        break;
      case 't':
        args->t = 1;
        args->v = 1;
        break;
      case 'T':
        args->t = 1;
        break;
      case 'v':
        args->v = 1;
        break;
      case '?':
        fprintf(stderr, "Try \'cat --help\' for more information.\n");
        return -1;
      default:
        fprintf(stderr, "Unknown error\n");
        return -1;
    }
  }
  return optind;
}

int main(int argc, char **argv) {
  Arguments args = {0};
  args.number = 1;
  args.slashN = 0;
  int first_file = parseArguments(argc, argv, &args);
  if (first_file == -1) {
    return -1;
  }

  args.print_file_name = argc - first_file <= 1 ? 0 : 1;

  for (int i = first_file; i < argc; i++) {
    processFile(argv[i], &args, i == first_file ? 1 : 0);
  }
}
