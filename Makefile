# Makefile for Legends of Arkania
# C++ RPG Game Project

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -O2
TARGET = legends_of_arkania
SOURCES = main.cpp Player.cpp Enemy.cpp Battle.cpp Map.cpp Shop.cpp Game.cpp Colors.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = Player.h Enemy.h Battle.h Map.h Shop.h Game.h Colors.h

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)
	@echo "Build complete! Run ./$(TARGET) to play."

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJECTS) $(TARGET)
	@echo "Clean complete."

# Run the game
run: $(TARGET)
	./$(TARGET)

# Create maps directory if it doesn't exist
maps:
	mkdir -p maps

# Install Python dependencies
python-deps:
	pip3 install matplotlib

# Help target
help:
	@echo "Legends of Arkania - Build System"
	@echo ""
	@echo "Available targets:"
	@echo "  make          - Build the game"
	@echo "  make run      - Build and run the game"
	@echo "  make clean    - Remove build artifacts"
	@echo "  make maps     - Create maps directory"
	@echo "  make python-deps - Install Python dependencies"
	@echo "  make help     - Show this help message"
	@echo ""
	@echo "Python Tools:"
	@echo "  python3 map_visualizer.py maps/<region>.txt [x] [y]"
	@echo "  python3 stats_analyzer.py savegame.txt [--plot]"

.PHONY: all clean run maps python-deps help

