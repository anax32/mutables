MKDIR = mkdir -p
CC = g++
FLAGS = -std=c++14 -g
INCLUDE_DIR=-I./include/ -I../utils/include/
LIBS=-lpng -lX11 -lGL -lGLEW

BIN_DIR = bin/
TEST_DIR = tests/
TEST_BIN_DIR = bin/test/

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

$(TEST_BIN_DIR): $(BIN_DIR)
	$(MKDIR) $(TEST_BIN_DIR)

test_tokens: $(TEST_DIR)mutables/tokens.cpp ./include/mutables/tokens.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_description: $(TEST_DIR)mutables/description.cpp ./include/mutables/description.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_property: $(TEST_DIR)mutables/property.cpp ./include/mutables/property.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_propertyset: $(TEST_DIR)mutables/propertyset.cpp ./include/mutables/propertyset.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_validators: $(TEST_DIR)mutables/validators.cpp ./include/mutables/validators.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) $(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

all_tests: test_tokens test_description test_validators test_property test_propertyset

all: all_tests

test_exes := $(wildcard $(TEST_BIN_DIR)*)

valgrind:
	@$(foreach test_exe, $(test_exes), valgrind -q $(test_exe);)

clean:
	rm -drf $(BIN_DIR)
