CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g -I src
TARGET = rmc7

SOURCES = src/lexer/Lexer.cpp \
          src/parser/Parser.cpp \
          src/parser/Helper.cpp \
          src/parser/ASTNode.cpp \
          src/parser/DeclarationNode.cpp \
          src/parser/DeclarationNodePrint.cpp \
          src/parser/ExpressionNode.cpp \
          src/parser/StatementNode.cpp \
          src/OutputWriter.cpp \
          src/Main.cpp
OBJECTS = $(SOURCES:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	@echo "[LD] $(TARGET)"
	@$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.cpp
	@echo "[CC] $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "[CLEAN]"
	@rm -f $(OBJECTS) $(TARGET)

run: all
	./$(TARGET)

.PHONY: all clean run
