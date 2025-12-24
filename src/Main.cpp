#include "./lexer/Lexer.h"
#include "./parser/Parser.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main()
{
    // Read file into string
    ifstream file("tests/test_input.c");
    
    if (!file.is_open()) {
        cout << "Error: Could not open file!" << endl;
        return 1;
    }
    
    // Read entire file content into string
    stringstream buffer;
    buffer << file.rdbuf();
    string sourceCode = buffer.str();
    
    file.close();
    
    // Create lexer with file content
    Lexer obj = Lexer(sourceCode);
    
    vector<Token> tokens = obj.startTokenization();
    
    cout << "Tokenization complete!\n";
    cout << "Found " << tokens.size() << " tokens\n";
    
    // Write tokens to output file
    ofstream outFile("output.txt");
    
    if (!outFile.is_open()) {
        cout << "Error: Could not create output file!" << endl;
        return 1;
    }
    
    outFile << "========================================\n";
    outFile << "          TOKENIZATION OUTPUT          \n";
    outFile << "========================================\n\n";
    outFile << "Total Tokens: " << tokens.size() << "\n\n";
    outFile << "----------------------------------------\n\n";
    
    for (size_t i = 0; i < tokens.size(); i++) {
        outFile << "Token #" << (i + 1) << ":\n";
        outFile << "  Type:   " << tokenTypeToString(tokens[i].type) << "\n";
        outFile << "  Data:   \"" << tokens[i].data << "\"\n";
        outFile << "  Line:   " << tokens[i].line << "\n";
        outFile << "  Column: " << tokens[i].column << "\n";
        outFile << "----------------------------------------\n";
    }
    
    outFile.close();
    
    cout << "Tokens written to output.txt\n";

    Parser myParser = Parser(tokens);

    ProgramNode* myRootAST = myParser.startParsing();
    
    return 0;
}