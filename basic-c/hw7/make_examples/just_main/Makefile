CC = gcc
CFLAGS = -Wall -Wextra -Werror -I include

OBJ = main.o
TARGET = program

# program
$(TARGET): $(OBJ) # main.o
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# main.o
$(OBJ): main.c
	$(CC) $(CFLAGS) -c main.c -o $(OBJ)

clean:
	rm $(OBJ) $(TARGET)
