CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

TARGET = crond

SRC = src/main.c \
      src/scheduler.c \
      src/process.c \
      src/ipc.c

OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

pipe_test:
	$(CC) $(CFLAGS) tests/pipe_test.c -o pipe_test

fifo_test:
	$(CC) $(CFLAGS) tests/fifo_test.c -o fifo_test

signal_test:
	$(CC) $(CFLAGS) tests/signal_test.c -o signal_test

tests: pipe_test fifo_test signal_test

.PHONY: all clean tests pipe_test fifo_test signal_test
