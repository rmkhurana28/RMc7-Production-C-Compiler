#ifndef HELPER_H
#define HELPER_H

#include "../lexer/Token.h"
#include <vector>
#include <string>
#include "ASTNode.h"    
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

// Forward declarations for circular dependency
class ParameterNode;

// struct for storing the var name data
typedef struct varNameProp{
    nameTypeSpecifier type;
    
    string varName; // to store var name
    short unsigned numPointor; // to store number of pointors
    
    // For ARRAY:
    ASTNode* arrayExpr; // to store the array size expression
    
    // For FUNC:
    vector<ParameterNode> funcParams; // to store function parameters
    bool isVariadic; // true if function has ...
} varNameProp;

// Helper classes for parsing declarations

class Parser;  // Forward declaration
class varNameHolder;  // Forward declaration
class VariableDeclarationNode;  // Forward declaration

class dataTypeHolder{
    friend class varNameHolder;  // Allow varNameHolder to access private members
    friend class VariableDeclarationNode;  // Allow VariableDeclarationNode to print
    friend class ParameterNode;  // Allow ParameterNode access for recursive printing
    friend void printParametersRecursive(ofstream&, const vector<ParameterNode>&, const string&);  // Allow helper function
    
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
    dataTypeHolder(const dataTypeHolder& other);
    void getDataType();

    int isCurrentTypeValid();
};


class varNameHolder{
    friend class VariableDeclarationNode;  // Allow VariableDeclarationNode to print
    friend class ParameterNode;  // Allow ParameterNode access for recursive printing
    friend void printParametersRecursive(ofstream&, const vector<ParameterNode>&, const string&);  // Allow helper function
private:
    Parser& parser; // reference to main parser object

    vector<varNameProp> namePropArray; // to store the name properties

    bool isArray; // is array or not
    short arrayDimensions = 0; // number of dimensions if array
    
public:
    varNameHolder(Parser& parser);
    varNameHolder(const varNameHolder& other);
    VariableDeclarationNode* getVarName(dataTypeHolder& typeHolder , bool isFuncParam);

    
};

#endif
