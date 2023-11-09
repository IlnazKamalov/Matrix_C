FLAGS = gcc -std=c11 -Wall -Werror -Wextra
OS=$(shell uname)

ifeq ($(OS),Darwin)
	LIBS=-lcheck -lm -lpthread
else
	LIBS=-lcheck -lm -lpthread -lrt -lsubunit
endif

all: clean s21_matrix.a

check:
	cp ../materials/linters/.clang-format ../src
	clang-format -n *.c *.h

s21_matrix.a:
	$(FLAGS) -c s21_matrix.c
	ar rc s21_matrix.a s21_matrix.o
	ranlib s21_matrix.a

test: clean s21_matrix.a
	$(FLAGS) s21_test_matrix.c s21_matrix.a -o s21_test $(LIBS)
	./s21_test

valcheck: clean s21_matrix.a
	$(FLAGS) s21_test_matrix.c s21_matrix.a -o s21_test $(LIBS)
	valgrind --leak-check=summary ./s21_test  >> val_result.log 2>&1

gcov_report: clean
	$(FLAGS) -fprofile-arcs -ftest-coverage s21_test_matrix.c s21_matrix.c -o s21_test $(LIBS)
	./s21_test
	lcov -t "test" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

clean:
	rm -rf .clang-format *.o *.out *.gcov *.gcno *.gcda *.a *.log *.info *.dSYM s21_test report val_result.log

rebuild: clean all

.PHONY: all, s21_matrix.a, clean, rebuild, check, valcheck, gcov_report, test