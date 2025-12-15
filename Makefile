CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I src
TARGET = rmc7

SOURCES = src/lexer/Lexer.cpp src/Main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
