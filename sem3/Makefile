.PHONY: \
	build \
	lint \
	all

all: lint build

lint:
	clang-format -i --Werror --style=file 2.c

build:
	gcc -std=c99 -Wall -Wextra -Werror 2.c -o out
