# Select compiler 
CC := g++

# Source, build and output locations
SRCDIR := src
BUILDDIR := build
BIN := bin
TARGET := $(BIN)/chess

SRCEXT := cc
SOURCES := $(shell find $(SRCDIR) -type f -name *.$(SRCEXT))
OBJECTS := $(patsubst $(SRCDIR)/%.$(SRCEXT),$(BUILDDIR)/%.o,$(SOURCES))

CFLAGS := -std=c++11 -g -Wall -Werror
LIB := -L/usr/local -lglfw3 -lglew -framework OpenGL
INC := -I include

# Linker
$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN)
	$(CC) $^ -o $@ $(LIB)

# Make object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT)
	@mkdir -p $(BUILDDIR)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(INC) -c -o $@ $<

# Clean
clean:
	$(RM) -r $(BUILDDIR) $(TARGET)

# Test
test:
	$(CC) $(CFLAGS) test/test.cpp $(INC) $(LIB) -o bin/test

.PHONY: clean
