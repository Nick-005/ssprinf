all: build buildtest

build:
	gcc -Wall -Werror -Wextra -std=c11 main.c -o s21_sprintf
	

buildtest:
	gcc -Wall -Werror -Wextra -std=c11 test.c -o test

run:
	./s21_sprintf.exe 


runtest:
	./test.exe