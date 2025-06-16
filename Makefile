# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

# Source files (only .cpp files — no .tpp or .hpp)
SRCS = main.cpp

# Output executable
TARGET = main

# Default build
all: $(TARGET)

# Link the final binary
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Clean build artifacts
clean:
	rm -f $(TARGET)