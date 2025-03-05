CC = g++

SRCS = main.cpp
CFLAGS = -Wall -O2 -g -std=c++17

OBJS = $(patsubst %.cpp, %.o, $(SRCS))

TARGET = out

# Helper src to object
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Make target home program
$(TARGET): lint $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

.PHONY: clean lint

lint:
	clang-format -i --Werror --style=file ${SRCS}

clean:
	rm -f $(OBJS) $(TARGET)

