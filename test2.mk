SOURCES = src/\[01\]\ lexer/Lexer.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(OBJECTS)

%.o: %.cpp
	@echo "CC $< to $@"

