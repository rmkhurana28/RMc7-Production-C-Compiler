#include "./lexer/Lexer.h"
#include "./parser/Parser.h"
#include "OutputWriter.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unordered_map>

extern unordered_map<string, vector<Token>> defMap;
extern unordered_map<string, FuncMacroConfig> funcDefMap;

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

    // {
    //         // --- Print Preprocessor #define HashMap ---
    // cout << "\n========================================\n";
    // cout << "         MACRO DEFINITIONS (defMap)     \n";
    // cout << "========================================\n";
    // cout << "Total Defined Macros: " << defMap.size() << "\n\n";

    // for (const auto& [name, macroTokens] : defMap) {
    //     cout << "#define " << name << " -> ";
    //     if (macroTokens.empty()) {
    //         cout << "(empty / flag macro)\n";
    //     } else {
    //         // Print raw expansion
    //         cout << "[ ";
    //         for (size_t i = 0; i < macroTokens.size(); i++) {
    //             cout << macroTokens[i].data;
    //             if (i + 1 < macroTokens.size()) cout << " ";
    //         }
    //         cout << " ]\n";

    //         // Print token breakdown (type & data)
    //         cout << "   Tokens: ";
    //         for (size_t i = 0; i < macroTokens.size(); i++) {
    //             cout << tokenTypeToString(macroTokens[i].type) 
    //                  << "(\"" << macroTokens[i].data << "\")";
    //             if (i + 1 < macroTokens.size()) cout << ", ";
    //         }
    //         cout << "\n";
    //     }
    //     cout << "----------------------------------------\n";
    // }
    // cout << endl;
    // }

        // --- Print Preprocessor Function-Like Macro HashMap ---
    cout << "\n========================================\n";
    cout << "   FUNCTION-LIKE MACROS (funcDefMap)    \n";
    cout << "========================================\n";
    cout << "Total Defined Function Macros: " << funcDefMap.size() << "\n\n";

    for (const auto& [name, config] : funcDefMap) {
        cout << "#define " << name << "(";
        
        // Print arguments
        if (config.args.empty()) {
            cout << "";
        } else {
            bool first = true;
            for (const auto& arg : config.args) {
                if (!first) cout << ", ";
                cout << arg;
                first = false;
            }
        }
        cout << ") -> ";

        // Print replacement tokens
        if (config.tokList.empty()) {
            cout << "(empty body)\n";
        } else {
            cout << "[ ";
            for (size_t i = 0; i < config.tokList.size(); i++) {
                cout << config.tokList[i].data;
                if (i + 1 < config.tokList.size()) cout << " ";
            }
            cout << " ]\n";

            // Print token breakdown
            cout << "   Tokens: ";
            for (size_t i = 0; i < config.tokList.size(); i++) {
                cout << tokenTypeToString(config.tokList[i].type) 
                     << "(\"" << config.tokList[i].data << "\")";
                if (i + 1 < config.tokList.size()) cout << ", ";
            }
            cout << "\n";
        }
        cout << "----------------------------------------\n";
    }
    cout << endl;
    
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