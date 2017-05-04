CC := gcc
CFLAGS := -fPIC -Wall -Wextra -O2 -g
LDFLAGS := -shared -ldl

TARGET = libwgotw.so

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

clean:
	rm -f $(TARGET) $(OBJS)
