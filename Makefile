CXX = g++
CXXFLAGS = -std=c++14 $(INCLUDE)
INCLUDE = -Iinclude/ -I../utils/include/ -I../maketest/
LDLIBS = -lpng -lX11 -lGL -lGLEW

TEST_DIR = tests/
include ../maketest/Makefile
