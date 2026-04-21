# MERIX Compiler Makefile
# Usage: make          -> builds the compiler
#        make clean    -> removes build artifacts
#        make debug    -> builds with debug symbols and no optimization
#        make release  -> builds with optimizations

# Compiler and flags
CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -pedantic -I src/header
LDFLAGS  :=

# Directories
SRC_DIR  := src/code
LIB_DIR := src/lib/code
BUILD_DIR := build
TARGET   := merix

# Collect .cpp files under src/code AND src/main.cpp
SOURCES  := $(shell find $(SRC_DIR) -name '*.cpp') src/main.cpp
# Convert to object files in build/ directory
OBJECTS  := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(filter $(SRC_DIR)/%.cpp, $(SOURCES)))
OBJECTS  += $(BUILD_DIR)/main.o

# Default target
all: $(TARGET)

# Link the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Build complete: $(TARGET)"

# Compile .cpp to .o for files under src/code/
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile .cpp to .o for files under src/lib/code
$(BUILD_DIR)/%.o: $(LIB_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile src/main.cpp separately
$(BUILD_DIR)/main.o: src/main.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Debug build
debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

# Release build
release: CXXFLAGS += -O3 -DNDEBUG
release: clean all

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean debug release