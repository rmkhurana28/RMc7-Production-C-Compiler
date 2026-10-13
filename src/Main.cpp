#include "./lexer/Lexer.h"
#include "./parser/Parser.h"
#include "OutputWriter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>
extern unordered_map<string, vector<Token>> defMap;

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

    {
            // --- Print Preprocessor #define HashMap ---
    cout << "\n========================================\n";
    cout << "         MACRO DEFINITIONS (defMap)     \n";
    cout << "========================================\n";
    cout << "Total Defined Macros: " << defMap.size() << "\n\n";

    for (const auto& [name, macroTokens] : defMap) {
        cout << "#define " << name << " -> ";
        if (macroTokens.empty()) {
            cout << "(empty / flag macro)\n";
        } else {
            // Print raw expansion
            cout << "[ ";
            for (size_t i = 0; i < macroTokens.size(); i++) {
                cout << macroTokens[i].data;
                if (i + 1 < macroTokens.size()) cout << " ";
            }
            cout << " ]\n";

            // Print token breakdown (type & data)
            cout << "   Tokens: ";
            for (size_t i = 0; i < macroTokens.size(); i++) {
                cout << tokenTypeToString(macroTokens[i].type) 
                     << "(\"" << macroTokens[i].data << "\")";
                if (i + 1 < macroTokens.size()) cout << ", ";
            }
            cout << "\n";
        }
        cout << "----------------------------------------\n";
    }
    cout << endl;
    }
    
    ofstream outFile("output.txt");
    if (!outFile.is_open()) return 1;
    
    OutputWriter::writeTokensToFile(tokens, outFile);
    // OutputWriter::writeTokensToFile(rawTokens, outFile);
    
    // Parser myParser = Parser(rawTokens);
    Parser myParser = Parser(tokens);

    myStack.push_back(GLOBAL); // manually push GLOABL representing the starting point of phase-2

    // ProgramNode* myRootAST = myParser.startParsing();
    
    // if(myRootAST != nullptr) {
    //     myRootAST->printAST(outFile);
    // }
    
    // // Print expression statements
    // printStatementsToFile(outFile, myParser.getStatements());
    
    outFile.close();
    
    return 0;
}