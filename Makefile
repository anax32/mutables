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

test_tokens: $(TEST_DIR)mutables/tokens.cpp $(INCLUDE_DIR)mutables/tokens.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_description: $(TEST_DIR)mutables/description.cpp $(INCLUDE_DIR)mutables/description.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_property: $(TEST_DIR)mutables/property.cpp $(INCLUDE_DIR)mutables/property.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_propertyset: $(TEST_DIR)mutables/propertyset.cpp $(INCLUDE_DIR)mutables/propertyset.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

test_validators: $(TEST_DIR)mutables/validators.cpp $(INCLUDE_DIR)mutables/validators.h $(TEST_BIN_DIR)
	$(CC) $(FLAGS) -I$(INCLUDE_DIR) $(LIBS) -o $(TEST_BIN_DIR)$@ $<

all_tests: test_mutable_tokens test_mutable_description test_mutable_validators test_mutable_property test_mutable_propertyset

all: all_tests

test_exes := $(wildcard $(TEST_BIN_DIR)*)

valgrind:
	@$(foreach test_exe, $(test_exes), valgrind -q $(test_exe);)

clean:
	rm -drf $(BIN_DIR)
