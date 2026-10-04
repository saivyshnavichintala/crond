CC = gcc

CFLAGS = -Wall -Wextra -Iinclude

TARGET = crond

SOURCES = \
	src/main.c \
	src/scheduler.c \
	src/process.c

all:
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -f $(TARGET)

run: all
	./$(TARGET)

test:
	$(CC) $(CFLAGS) tests/test_scheduler.c -o test_scheduler
	./test_scheduler
	rm -f test_scheduler
