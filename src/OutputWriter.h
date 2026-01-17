#ifndef OUTPUTWRITER_H
#define OUTPUTWRITER_H

#include <vector>
#include <string>
#include <fstream>
#include "./lexer/Token.h"

using namespace std;

class OutputWriter {
public:
    static void writeTokensToFile(const vector<Token>& tokens, ofstream& outFile);
};

#endif
