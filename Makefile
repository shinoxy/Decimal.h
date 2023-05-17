CFLAGS =gcc -Wall -Wextra -Werror -std=c11
GCOVFLAGS = --coverage 

ifeq ($(shell uname -s), Linux) 
	LFLAG += -lpthread -lrt -lm -lsubunit
endif

all: s21_decimal.a

s21_decimal.a:
	gcc -Wall -Wextra -Werror -std=c11 -c s21_*.c
	ar rc s21_decimal.a s21_*.o
	ranlib s21_decimal.a

test: clean s21_decimal.a 
	gcc tests/*.c -o test s21_decimal.a -lcheck $(LFLAG)
	./test

leaks: test
	valgrind -s --leak-check=full --track-origins=yes --show-leak-kinds=all ./test

fsanitize: clean
	gcc -Wall -Wextra -Werror -fsanitize=address -g -std=c11 -c s21_*.c
	ar rc s21_decimal.a s21_*.o
	ranlib s21_decimal.a
	gcc -fsanitize=address -g TEST.c -o test s21_decimal.a -lcheck $(LFLAG)
	./test

gcov_report: clean
	gcc s21_*.c TEST.c -lcheck -o gcov_report --coverage $(LFLAG)
	./gcov_report
	rm -rf *test*.gc* 
	gcov *s21_*.c 
	lcov -d . -o gcov_report.info -c
	genhtml -o report gcov_report.info
	open report/index.html

style:
	clang-format -style=Google -i *.c *.h
	clang-format -style=Google -n *.c *.h

rebuild: clean all

clean:
	rm -f *.gcno *.gcda *.info *.gcov 
	rm -f *.o *.a
	rm -rf report *.dSYM
	rm -f *.out
	rm -rf test
	rm -f gcov_report
