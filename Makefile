# Compiler, Linker Flags, Source Properties
CC := g++
CFLAGS := -std=c++11 -g -O3 -Wall -Werror
LFLAGS := -L/usr/local -L lib 
SRCEXT := cc

# Required Folders
SRC := src
BIN := bin
INC := -I include -I . -I ./src -I ./src/gl
LIB := -lboost_serialization -lglfw3 -lglew -framework OpenGL
BUILD := build

# Test Dependencies
TEST := test
TLIB := $(LIB) -pthread -L lib -lgmock

# Build Targets
TARGET_TEXT := $(BIN)/chess-text
TARGET_DRAW := $(BIN)/chess-draw
TARGET_TEST := $(BIN)/chess-test
TEXT_RUNNER := $(BUILD)/main/chess_text.o
DRAW_RUNNER := $(BUILD)/main/chess_draw.o

# Load sources and objects
SOURCES := $(shell find $(SRC) -type f -name *.$(SRCEXT) ! -path "*/main/*")
OBJECTS := $(patsubst $(SRC)/%.$(SRCEXT),$(BUILD)/%.o,$(SOURCES))
TESTS	:= $(shell find $(TEST) -type f -name *.$(SRCEXT))
TESTOBJ := $(filter-out $(BUILD)/*.o, $(OBJECTS))

# All
all: $(TARGET_TEXT) $(TARGET_DRAW)

# Link chess-text (bin/chess-text)
$(TARGET_TEXT): $(TEXT_RUNNER) $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ $(LFLAGS) $(LIB)

# Link chess-draw (bin/chess-draw)
$(TARGET_DRAW): $(DRAW_RUNNER) $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ $(LFLAGS) $(LIB)

# Compile (*.o)
$(BUILD)/%.o: $(SRC)/%.$(SRCEXT)
	@mkdir -p $(BUILD)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Clean
clean:
	$(RM) -r $(BUILD) $(BIN)

# Tester
test: $(OBJECTS)
	@echo " $(TOBJ)"
	$(CC) $(CFLAGS) $(TESTS) $(TESTOBJ) $(INC) $(LFLAGS) $(TLIB) -o $(TARGET_TEST)

.PHONY: clean
