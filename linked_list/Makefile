# === Compiler and Flags ===
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2

# === Project Files ===
SRCS := $(wildcard *.cpp)
OBJS := $(SRCS:.cpp=.o)
TARGET := browser_cache_demo

# === Default rule ===
all: $(TARGET)

# === Linking rule ===
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# === Compilation rule ===
%.o: %.cpp *.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Cleanup ===
clean:
	rm -f $(OBJS) $(TARGET)

# === Convenience ===
run: $(TARGET)
	./$(TARGET)
