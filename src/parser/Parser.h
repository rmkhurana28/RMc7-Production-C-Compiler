#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../lexer/Token.h"
#include "ASTNode.h"
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "DeclarationNode.h"

using namespace std;

class Parser {
    friend class dataTypeHolder; // allows dataTypeHolder to access private section of this class
private:
    vector<Token> tokens;
    size_t currentPos;    
    
    // Add your helper methods here
    DeclarationNode* parseCurrentDecl();

    // check
    bool isThisTokenDataTypeOrPropToken(Token currToken);
    bool isThisTokenDataBaseTypeToken(Token currToken);
    bool isThisTokenSignModifierToken(Token currToken);
    bool isThisTokenSizeModifierToken(Token currToken);
    bool isThisTokenTypeQualifierToken(Token currToken);
    bool isThisTokenStorageClassToken(Token currToken);

    DeclarationNode* parseDataTypeFoundDeclaration();

    bool findDataTypePropCombinationAndValidate(Token currToken);
    
public:
    Parser(const vector<Token>& tokenList);
    ~Parser();
    
    // Main parsing method
    ProgramNode* startParsing();

    vector<Token> getTokenArray(){
        return tokens;
    }

    
    
    // Add your parsing methods here (parseExpression, parseStatement, etc.)
};

#endif
