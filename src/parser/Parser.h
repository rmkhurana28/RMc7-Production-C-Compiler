#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "../lexer/Token.h"
#include "ASTNode.h"
#include "ExpressionNode.h"
#include "StatementNode.h"
#include "DeclarationNode.h"

#include <unordered_map>

using namespace std;

class Parser {
    friend class dataTypeHolder; // allows dataTypeHolder to access private section of this class
    friend class varNameHolder; // allows varNameHolder to access private section of this class
private:
    vector<Token> tokens;
    size_t currentPos;
    
    // type registry for the struct/union/enum data types
    unordered_map<string, string> typeRegisry;

    // hashmap to store new->old[] typedef alias
    unordered_map<string, vector<string>> tdMap;
    
    // Add your helper methods here
    DeclarationNode* parseCurrentDecl();

    // check
    bool isThisTokenDataTypeOrPropToken(Token currToken);
    bool isThisTokenDataBaseTypeToken(Token currToken);
    bool isThisTokenSignModifierToken(Token currToken);
    bool isThisTokenSizeModifierToken(Token currToken);
    bool isThisTokenTypeQualifierToken(Token currToken);
    bool isThisTokenStorageClassToken(Token currToken);

    bool isThisTokenStructUnionEnumToken(Token currToken);

    DeclarationNode* parseDataTypeFoundDeclaration();

    bool isThisStringPresentAsKeyInTrHm(string key);
    bool isThisStringPresentAsKeyInTdMap(string key);

    bool isCurrentIdValidTdAlias();

    bool findDataTypePropCombinationAndValidate(Token currToken);
    
public:
    Parser(const vector<Token>& tokenList);
    ~Parser();
    
    // Main parsing method
    ProgramNode* startParsing();

    Token getCurrentToken();

    vector<Token> getTokenArray(){
        return tokens;
    }

    
    
    // Add your parsing methods here (parseExpression, parseStatement, etc.)
};

#endif
