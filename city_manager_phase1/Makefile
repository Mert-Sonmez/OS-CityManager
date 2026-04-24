CC = gcc
CFLAGS = -Wall -Wextra -std=c99

TARGET = city_manager

SRCS = city_manager.c file_io.c operations.c permissions.c filter.c

OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c city_manager.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)