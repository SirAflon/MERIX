CXX      := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -pedantic -I src/header
LDFLAGS  :=

SRC_DIR   := src/code
LIB_DIR   := src/lib/code
BUILD_DIR := build
TARGET    := merix

SOURCES := $(shell find $(SRC_DIR) $(LIB_DIR) -name '*.cpp') src/main.cpp
OBJECTS := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Build complete: $(TARGET)"

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: clean all

release: CXXFLAGS += -O3 -march=native -flto -DNDEBUG
release: LDFLAGS  += -flto
release: clean all

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all clean debug release