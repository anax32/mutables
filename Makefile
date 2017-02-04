CC=g++
FLAGS=-std=c++11
INCLUDE_DIR=include/
LIBS=-lpng -lX11 -lGL -lGLEW

TEST_INPUT=tests/tests.cpp
OUT_DIR=bin/

dirs:
	mkdir -p $(OUT_DIR)

test: $(TEST_INPUT)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) $< -o ${OUT_DIR}/$@

all: dirs test
