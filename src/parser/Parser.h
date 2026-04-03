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

typedef struct tdMapPair{
    vector<string> declProp;
    varNameHolder* nameProp;
} tdMapPair;

extern vector<ASTNode*> tempASTStorage; // to store ast which are diffcult to handle using recursive-decent approach, need custom additon

extern bool typedDefTracker;

typedef enum locationStack{
    GLOBAL, // acting as a prent fro below 2
    GLOABL_VAR_DECL, // global var decl starting point
    GLOBAL_VAR_INIT, // gloabl var init , after decl starting point

    TYPEDEF, // acting as parent for below 2
    TYPEDEF_ORIGINAL, // typedef original starting poiny
    TYPEDEF_ALIAS, // typedef alias starting point

    STRUCT, // acting as parent for below 4
    STRUCT_TAGNAME, // struct tag name starting point
    STRUCT_BLOCK, // struct block starting point
    STRUCT_VAR_NAME, // struct varName starting point
    STRUCT_TYPEDEF_ALIAS, // struct with typedef alias

    UNION, // acting as parent for below 4
    UNION_TAGNAME, // union tag name starting point
    UNION_BLOCK, // union block starting point
    UNION_VAR_NAME, // union varName starting point
    UNION_TYPEDEF_ALIAS, // union with typedef alias

    ENUM, // acting as parent for below 4
    ENUM_TAGNAME, // enum tag name starting point
    ENUM_BLOCK, // enum block starting point
    ENUM_VAR_NAME, // enum varName starting point
    ENUM_TYPEDEF_ALIAS, // enum with typedef alias

    FUNCC, // acting as parent for below 4
    FUNCC_DECL, // func decl starting point
    FUNCC_CALL, // func call starting point
    FUNCC_PARAMS, // func param starting point
    FUNCC_DEFI_BLOCK, // func definition block starting point

    IF, // acting as parent fro below 2
    IF_COND, // if cond starting point 
    IF_BLOCK, // if block starting point
    
    ELSE, // acting as parent for below 1
    ELSE_BLOCK, // else block starting point    

    WHILE, // acting as parent for below 2
    WHILE_COND, // while cond starting point
    WHILE_BLOCK, // while block starting point

    DO_WHILE, // acting as parent for below 2
    DO_WHILE_BLOCK, // do-while block starting point
    DO_WHILE_COND, // do-while cond starting point

    FOR, // acting as parent for below 4
    FOR_INIT, // for init starting point
    FOR_COND, // for cond starting point
    FOR_INCR, // for incr starting point
    FOR_BLOCK, // for block starting point
    
    SWITCH, // acting as parent for below 2
    SWITCH_COND, // switch cond starting point
    SWITCH_BLOCK, // switch block starting point
    CASE, // acting as parent for below 2
    CASE_COND, // case cond starting point
    CASE_BLOCK, // case block starting point
    DEFAULT, // acting as parent fro below 1
    DEFAULT_BLOCK, // default block starting point

    DECL, // acting as parent for below 1
    DECL_VAR, // var decl starting point

    EXPR, // expression starting point
    
} locationStack;

// global variable to store the stack
extern vector<locationStack> myStack;

class Parser {
    friend class dataTypeHolder; // allows dataTypeHolder to access private section of this class
    friend class varNameHolder; // allows varNameHolder to access private section of this class
    friend class ParameterNode;
    friend BlockExpressionNode* parseBlock(Parser& parser); // allows parseBlock function to access private members
private:
    vector<Token> tokens;
    size_t currentPos;

    // array to store all the AST
    vector<ASTNode*> allAST;

    // array to store all declaration nodes
    vector<DeclarationNode*> allDeclNodes;
    
    // array to store all statements (expression statements, control flow, etc.)
    vector<StatementNode*> allStmts;
    
    // type registry for the struct/union/enum data types
    unordered_map<string, string> typeRegisry;

    // hashmap to store new->old[] typedef alias
    // unordered_map<string, vector<string>> tdMap;
    unordered_map<string, vector<tdMapPair>> tdMap;

    ASTNode** highLevelParse(); // to generate all the AST and return accordingly (current plan is to return an array of ast nodes, might need to add all of them to the high level array)
    
    // Add your helper methods here
    DeclarationNode* parseCurrentDecl();
    StatementNode* parseExpressionStatement();
    ASTNode** startParsingOfCurrentToken();

    // check
    bool isThisTokenDataTypeOrPropToken(Token currToken);
    bool isThisTokenDataBaseTypeToken(Token currToken);
    bool isThisTokenSignModifierToken(Token currToken);
    bool isThisTokenSizeModifierToken(Token currToken);
    bool isThisTokenTypeQualifierToken(Token currToken);
    bool isThisTokenStorageClassToken(Token currToken);

    bool isThisTokenStructUnionEnumToken(Token currToken);

    ASTNode** parseDataTypeFoundDeclaration();

    bool isThisStringPresentAsKeyInTrHm(string key);
    bool isThisStringPresentAsKeyInTdMap(string key);

    bool isCurrentIdValidTdAlias();

    bool findDataTypePropCombinationAndValidate(Token currToken);


    // Expression parsing method
    ExpressionNode* parseExpression(short initPrec , bool stopAtComma, int needManualPushOfBrackets);

    // Control flow statements
    StatementNode* parseIf();
    StatementNode* parseFor();
    StatementNode* parseWhile();
    StatementNode* parseDoWhile();
    StatementNode* parseSwitch();
    StatementNode* parseCaseLabel();
    StatementNode* parseDefaultLabel();
    
    // Structured types
    ASTNode** parseStruct(dataTypeHolder* helperDeclName);
    DeclarationNode* parseEnum();
    DeclarationNode* parseUnion();
    ASTNode** parseTypedef();
    
    // Jump statements
    StatementNode* parseReturn();
    StatementNode* parseContinue();
    StatementNode* parseBreak();
    StatementNode* parseGoto();
    StatementNode* parseLabel();

    
public:
    Parser(const vector<Token>& tokenList);
    ~Parser();
    
    // Main parsing method
    ProgramNode* startParsing();

    Token getCurrentToken();

    vector<Token> getTokenArray(){
        return tokens;
    }
    
    vector<StatementNode*> getStatements(){
        return allStmts;
    }


    bool isThisParenForTypeCast();
    
    
    // Add your parsing methods here (parseExpression, parseStatement, etc.)
};

#endif
