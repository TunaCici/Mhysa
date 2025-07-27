# Makefile

PYTHON_CFLAGS := $(shell python3-config --cflags)
PYTHON_LDFLAGS := $(shell python3-config --ldflags --embed)

TARGET = cherry_app
SRCS = main.c cherry.c
OBJS = $(SRCS:.c=.o)

CC = clang
CFLAGS = -Wall -Wextra -O2 $(PYTHON_CFLAGS)
LDFLAGS = $(PYTHON_LDFLAGS)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c cherry.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean