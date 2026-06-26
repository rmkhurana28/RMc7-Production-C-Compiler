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
    
    Lexer obj = Lexer(sourceCode , "test_input.c");
    vector<Token> rawTokens = obj.startTokenization();    
    vector<Token> tokens = obj.startPreprocessing(rawTokens);
    
    ofstream outFile("output.txt");
    if (!outFile.is_open()) return 1;
    
    OutputWriter::writeTokensToFile(tokens, outFile);
    // OutputWriter::writeTokensToFile(rawTokens, outFile);
    
    Parser myParser = Parser(tokens);

    myStack.push_back(GLOBAL); // manually push GLOABL representing the starting point of phase-2

    // ProgramNode* myRootAST = myParser.startParsing();
    
    // if(myRootAST != nullptr) {
    //     myRootAST->printAST(outFile);
    // }
    
    // Print expression statements
    // printStatementsToFile(outFile, myParser.getStatements());
    
    outFile.close();
    
    return 0;
}