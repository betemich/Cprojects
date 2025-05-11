CFLAGS = -Wall -Wextra -Werror -g -std=c11
TEST_FLAGS = -lcheck -lsubunit -lm
GCOV_FLAGS = --coverage
LCOV_FLAGS = --capture --directory .

CDIR = ./Cfiles
TESTDIR = ./TestFiles
HEADER = s21_string.h

CFILES = $(wildcard $(CDIR)/*.c)
OFILES = $(patsubst %.c, %.o, $(CFILES))

LIBFILE = s21_string.a
TESTFILE = $(TESTDIR)/test.c
TEST_OBJ_FILE = $(TESTDIR)/test.out
TEST_RESULT_FILE = $(TESTDIR)/result.log
TEST_VALGRIND_RESULT = $(TESTDIR)/valgrind_result.log
INFO_FILE = $(TESTDIR)/coverage.info

%.o: %.c $(HEADER)
	gcc $(CFLAGS) -c $< -o $@

all: s21_string.a test gcov_report

s21_string.a: $(OFILES)
	ar rcs s21_string.a $(OFILES)
	rm -rf $(OFILES)
	
test:
	gcc $(CFILES) $(TESTFILE) $(CFLAGS) $(TEST_FLAGS) -o $(TEST_OBJ_FILE)
	$(TEST_OBJ_FILE) > $(TEST_RESULT_FILE)

gcov_report:
	gcc $(GCOV_FLAGS) $(CFILES) $(TESTFILE) $(CFLAGS) $(TEST_FLAGS) -o $(TEST_OBJ_FILE)
	$(TEST_OBJ_FILE) > $(TEST_RESULT_FILE)
	rm -rf $(TEST_OBJ_FILE)-test*
	lcov $(LCOV_FLAGS) -o $(INFO_FILE) --verbose
	genhtml --output-directory $(TESTDIR)/report --legend $(INFO_FILE) --ignore-errors unmapped,unmapped
	open $(TESTDIR)/report/index.html
	rm -rf $(TESTDIR)/*.gcno $(TESTDIR)/*.gcda $(TESTDIR)/*.info

valgrind:
	valgrind --leak-check=yes --quiet  $(TEST_OBJ_FILE) 2>&1

clang-format:
	clang-format -n $(CFILES) $(HEADER) $(TESTDIR)/test.c

clean:
	rm -rf $(TESTDIR)/*.log $(TESTDIR)/*.out $(TESTDIR)/*.info $(TESTDIR)/*.gcno $(TESTDIR)/*.gcda $(CDIR)/*.out $(CDIR)/*.log $(CDIR)/*.o
	rm -rf $(TESTDIR)/report *.a


	
