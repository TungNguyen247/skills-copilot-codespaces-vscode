# test/Makefile
# Build and run host-side mock tests for main.c (Nucleo-G474RE LED blink).
#
# Usage:
#   make          -- build and run
#   make clean    -- remove build artifacts

CC      = gcc
CFLAGS  = -Wall -Wextra -Wpedantic -std=c11 \
          -I. \
          -DTEST_BUILD

SRCS    = mock_hal.c test_led_blink.c
TARGET  = test_led_blink

.PHONY: all clean

all: $(TARGET)
	./$(TARGET)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(TARGET)
