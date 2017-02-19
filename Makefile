MKDIR = mkdir -p
CC = g++
FLAGS = -std=c++14
INCLUDE_DIR=include/
LIBS=-lpng -lX11 -lGL -lGLEW

BIN_DIR = bin/
TEST_DIR = test/
TEST_BIN_DIR = bin/test/

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

$(TEST_BIN_DIR): $(BIN_DIR)
	$(MKDIR) $(TEST_BIN_DIR)

test_gl_context: $(TEST_DIR)gl/context.cpp $(INCLUDE_DIR)gl/context.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $(TEST_DIR)gl/context.cpp

test_gl_textures: $(TEST_DIR)gl/texture.cpp $(INCLUDE_DIR)gl/texture.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $(TEST_DIR)gl/texture.cpp

test_png_io: $(TEST_DIR)img_io/png_io.cpp $(INCLUDE_DIR)img_io/png_io.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $(TEST_DIR)img_io/png_io.cpp

all_tests: test_gl_context test_gl_textures test_png_io

all: all_tests