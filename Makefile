MKDIR = mkdir -p
CC = g++
FLAGS = -std=c++14 -g
INCLUDE_DIR=include/
LIBS=-lpng -lX11 -lGL -lGLEW

BIN_DIR = bin/
TEST_DIR = tests/
TEST_BIN_DIR = bin/test/

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

$(TEST_BIN_DIR): $(BIN_DIR)
	$(MKDIR) $(TEST_BIN_DIR)

test_gl_context: $(TEST_DIR)gl/context.cpp $(INCLUDE_DIR)gl/context.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_textures: $(TEST_DIR)gl/texture.cpp $(INCLUDE_DIR)gl/texture.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_png_io: $(TEST_DIR)img_io/png_io.cpp $(INCLUDE_DIR)img_io/png_io.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_framebuffers: $(TEST_DIR)gl/framebuffer.cpp $(INCLUDE_DIR)gl/framebuffer.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_buffer: $(TEST_DIR)gl/buffer.cpp $(INCLUDE_DIR)gl/buffer.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_shader: $(TEST_DIR)gl/shader.cpp $(INCLUDE_DIR)gl/shader.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_parameterset: $(TEST_DIR)structures/parameterset.cpp $(INCLUDE_DIR)structures/parameterset.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_bounds: $(TEST_DIR)structures/bounds.cpp $(INCLUDE_DIR)structures/bounds.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

all_gl_tests: test_gl_context test_gl_textures test_gl_framebuffers test_gl_buffer test_gl_shader
all_io_tests: test_png_io
all_structests: test_parameterset test_bounds

all_tests: all_gl_tests all_io_tests all_structests

all: all_tests

clean:
	rm -drf $(BIN_DIR)