TARGET := evaluator

CFLAGS := -Wpedantic -Wall -Wextra -std=c99

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(TARGET)
