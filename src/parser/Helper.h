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
    
    vector<TokenType> trKeywordArray; // array to store keyword for the type registry 
    vector<string> trBaseArray; // array to store the new base type
    vector<string> tdNew; // array to store the new name of keywords used

    bool isPrevTokenValidForCurrentStar(TokenType prevTokenType);

    
    

public:
    dataTypeHolder(Parser& parser);
    void getDataType();

    int isCurrentTypeValid();
};

#endif
