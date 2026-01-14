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

// enum for name validaiton type
enum nameTypeSpecifier{
    VAR_NAME, // var name
    POINTOR, // pointor
    ARRAY, // array
    FUNC, // func
};

// struct for storing the var name data
typedef struct varNameProp{
    nameTypeSpecifier type;
    
    string varName; // to store var name
    short unsigned numPointor; // to store number of pointors
    ASTNode* astData; // to store the params if func or to store the arrSize
} varNameProp;

// Helper classes for parsing declarations

class Parser;  // Forward declaration
class varNameHolder;  // Forward declaration

class dataTypeHolder{
    friend class varNameHolder;  // Allow varNameHolder to access private members
    
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


class varNameHolder{
private:
    Parser& parser; // reference to main parser object

    vector<varNameProp> namePropArray; // to store the name properties

    bool isArray; // is array or not
    short arrayDimensions = 0; // number of dimensions if array
    
public:
    varNameHolder(Parser& parser);
    void getVarName(dataTypeHolder& typeHolder);

    
};

#endif
