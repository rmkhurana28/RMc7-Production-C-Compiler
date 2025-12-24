#ifndef HELPER_H
#define HELPER_H

#include "../lexer/Token.h"
#include <vector>
#include <string>
using namespace std;

// struct to store the star data
typedef struct starData{
    int numOfStars;
    TokenType typeBeforeStar;
} starData;

// Helper classes for parsing declarations

class Parser;  // Forward declaration

class dataTypeHolder{
private:
    Parser& parser; // reference to main parser object

    // possible data type properties and storage to store them
    vector<TokenType> baseTypeArray;
    vector<TokenType> signModifiersArray;
    vector<TokenType> sizeModifiersArray;
    vector<TokenType> typeQualifiersArray;
    vector<TokenType> storageClassArray;
    vector<starData> starDataArray;

    bool isPrevTokenValidForCurrentStar(TokenType prevTokenType);
    

public:
    dataTypeHolder(Parser& parser);
    void getDataType();

    bool isCurrentTypeValid();
};

#endif
