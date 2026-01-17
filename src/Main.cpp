#include "./lexer/Lexer.h"
#include "./parser/Parser.h"
#include "OutputWriter.h"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

int main()
{
    ifstream file("tests/test_input.c");
    if (!file.is_open()) return 1;
    
    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    file.close();
    
    Lexer obj = Lexer(sourceCode);
    vector<Token> tokens = obj.startTokenization();
    
    ofstream outFile("output.txt");
    if (!outFile.is_open()) return 1;
    
    OutputWriter::writeTokensToFile(tokens, outFile);
    
    Parser myParser = Parser(tokens);
    ProgramNode* myRootAST = myParser.startParsing();
    
    if(myRootAST != nullptr) {
        myRootAST->printAST(outFile);
    }
    
    outFile.close();
    
    return 0;
}