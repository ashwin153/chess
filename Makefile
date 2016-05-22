# Select compiler, flags and source properties
CC := g++
CFLAGS := -std=c++11 -g -Wall #-Werror
LFAGS  := -L/usr/local -L lib
SRCEXT := cc

# Production: source, build, include and output locations
SRC := src
BIN := bin
INC := -I include -I .
LIB := -lglfw3 -lglew -framework OpenGL
BUILD := build
TARGET := $(BIN)/chess

# Testing: source, build, include, and output locations
TEST := test
TLIB := $(LIB) -pthread -L lib -lgmock
TESTER := $(BIN)/tester

# Load sources and objects
SOURCES := $(shell find $(SRC) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRC)/%.$(SRCEXT),$(BUILD)/%.o,$(SOURCES))
TESTS	:= $(shell find $(TEST) -type f -name *.$(SRCEXT))

# All
all: $(TARGET)

# Linker (bin/chess)
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ $(LFLAG) $(LIB)

# Compile (*.o)
$(BUILD)/%.o: $(SRC)/%.$(SRCEXT)
	@mkdir -p $(BUILD)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Clean
clean:
	$(RM) -r $(BUILD) $(BIN)

# Tester
tester:
	$(CC) $(CFLAGS) $(TESTS) $(OBJECTS) $(INC) $(LFLAG) $(TLIB) -o $(TESTER)

.PHONY: clean
