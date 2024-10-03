.PHONY: \
	build \
	lint \
	all

BUILD_PATHS = I.c

all: lint build

lint:
	clang-format -i --Werror --style=file ${BUILD_PATHS}

build:
	gcc -std=c99 -Wall -Wextra -Werror ${BUILD_PATHS} -o out
