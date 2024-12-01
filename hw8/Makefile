CC = gcc

SRCS = 1.c
CFLAGS = -Wall -Wextra -Werror -I include

OBJS = $(patsubst %.c, %.o, $(SRCS))

TARGET = out

# Helper src to object
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Make target home program
$(TARGET): lint $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean lint

lint:
	clang-format -i --Werror --style=file ${SRCS}

clean:
	rm -f $(OBJS) $(TARGET)

