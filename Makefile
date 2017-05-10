CC := gcc
RM := rm -rf
CFLAGS := -fPIC -Wall -Wextra -Wno-unused-result -O2 -g -D_GNU_SOURCE
LDFLAGS := -shared -ldl

TARGET = libwgotw.so

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:.c=.o)

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

doc:
	VERS=$(shell git describe --tags --dirty --always 2>/dev/null \
			|| git rev-parse --short HEAD \
			|| echo "unknown-version") \
	doxygen Doxyfile

clean:
	$(RM) $(TARGET) $(OBJS)
	$(RM) html/ man/
