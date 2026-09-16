all: build buildtest

build:
	gcc -Wall -Werror -Wextra -std=c11 main.c s21_sprintf.c -o s21_sprintf -lm
	

buildtest:
	gcc -Wall -Werror -Wextra -std=c11 test.c -o abc -lm

test:
	gcc -Wall -Werror -Wextra -std=c11 main.c -o s21_sprintf -lm
	./s21_sprintf

old_test:
	gcc -Wall -Werror -Wextra -std=c11 main.c -o s21_sprintf -lm
	gcc -Wall -Werror -Wextra -std=c11 test.c -o abc -lm
	./s21_sprintf
	./abc	
run:
	./s21_sprintf


runtest:
	./abc

check:
	checkmk sprintf.check > test_string.c
	gcc -Wall -Wextra -Werror -std=c11 test_string.c main.c -lcheck -lsubunit -lm -o check.out