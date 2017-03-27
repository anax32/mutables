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

test_gl_context: $(TEST_DIR)gl/context.cpp $(INCLUDE_DIR)gl/context.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_textures: $(TEST_DIR)gl/texture.cpp $(INCLUDE_DIR)gl/texture.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_png_io: $(TEST_DIR)img_io/png_io.cpp $(INCLUDE_DIR)img_io/png_io.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_framebuffers: $(TEST_DIR)gl/framebuffer.cpp $(INCLUDE_DIR)gl/framebuffer.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_buffer: $(TEST_DIR)gl/buffer.cpp $(INCLUDE_DIR)gl/buffer.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_gl_shader: $(TEST_DIR)gl/shader.cpp $(INCLUDE_DIR)gl/shader.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_parameterset: $(TEST_DIR)structures/parameterset.cpp $(INCLUDE_DIR)structures/parameterset.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_bounds: $(TEST_DIR)structures/bounds.cpp $(INCLUDE_DIR)structures/bounds.h
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_callback: $(TEST_DIR)structures/callback.cpp $(INCLUDE_DIR)structures/callback.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_callbackset: $(TEST_DIR)structures/callbackset.cpp $(INCLUDE_DIR)structures/callbackset.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

all_gl_tests: test_gl_context test_gl_textures test_gl_framebuffers test_gl_buffer test_gl_shader
all_io_tests: test_png_io
all_structure_tests: test_parameterset test_bounds test_callback test_callbackset

all_tests: all_gl_tests all_io_tests all_structure_tests

all: all_tests

test_exes := $(wildcard $(TEST_BIN_DIR)*)

valgrind:
	@$(foreach test_exe, $(test_exes), valgrind -q $(test_exe);)

clean:
	rm -drf $(BIN_DIR)