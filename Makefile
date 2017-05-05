CC := gcc
CFLAGS := -fPIC -Wall -Wextra -Wno-unused-result -O2 -g -D_GNU_SOURCE
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
