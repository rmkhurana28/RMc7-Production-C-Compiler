#include "OutputWriter.h"
#include <iostream>

using namespace std;

void OutputWriter::writeTokensToFile(const vector<Token>& tokens, ofstream& outFile) {
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
}
