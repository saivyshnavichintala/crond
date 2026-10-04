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
