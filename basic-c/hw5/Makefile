CC = gcc

SRCS = 1.c
CFLAGS = -Wall -Wextra -Werror -I include

OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = out

# Helper src to object
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Make target home program
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean install uninstall test lint

lint:
	clang-format -i --Werror --style=file ${SRCS}

install: $(TARGET)
	cp $(TARGET) ./bin

uninstall:
	rm -f ./bin/$(TARGET)

TEST_SRCS = $(wildcard tests/*.c) $(filter-out src/seminar_main.c src/home_main.c, $(wildcard src/*.c))
TEST_OBJS = $(patsubst %.c, %.o, $(TEST_SRCS))
TEST_TARGET = test_home_program

test: $(TEST_TARGET)

$(TEST_TARGET): $(TEST_OBJS)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_OBJS)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(TEST_TARGET) $(TEST_OBJS)
