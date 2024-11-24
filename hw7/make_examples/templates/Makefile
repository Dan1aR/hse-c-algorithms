CC = gcc
CFLAGS = -Wall -Wextra -Werror -I include

# hello.c main.c
SRCS = $(wildcard *.c)
# hello.o main.o
OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = program

-include main.d

.PHONY: clean

$(TARGET): hello.o main.o
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm $(OBJS) $(TARGET)
