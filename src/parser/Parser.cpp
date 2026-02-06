#include "Parser.h"
#include <vector>
#include "Helper.h"
#include <iostream>

using namespace std;

Parser::Parser(const vector<Token>& tokenList) {
    tokens = tokenList;
    currentPos = 0;
}

Parser::~Parser() {
    // Clean up if needed
}

ProgramNode* Parser::startParsing() {
    // Main parsing logic will go here
    // This will return the root of the AST (ProgramNode)

    vector<DeclarationNode*> allDeclNodes; // array to store all the decl nodes

    // generate all relavent AST
    // keep parsing untill tokens are finished
    while(this->currentPos < tokens.size()){
        ASTNode* node = startParsingOfCurrentToken();
        
        if(node != nullptr) {
            // Check if it's a statement
            StatementNode* stmt = dynamic_cast<StatementNode*>(node);
            if(stmt != nullptr) {
                allStmts.push_back(stmt);
            }
            // Check if it's a declaration
            DeclarationNode* decl = dynamic_cast<DeclarationNode*>(node);
            if(decl != nullptr) {
                allDeclNodes.push_back(decl);
            }
        }
    }

    // generating program node containing an array of all decl nodes
    ProgramNode* myRootNode = new ProgramNode(allDeclNodes);

    // returning the program node as root node
    return myRootNode;
}

ASTNode* Parser::startParsingOfCurrentToken() {
    Token current = this->tokens[this->currentPos];
    
    // Check for control flow statements
    if(current.type == KEYWORD_IF) {
        return this->parseIf();
    } else if(current.type == KEYWORD_FOR) {
        return this->parseFor();
    } else if(current.type == KEYWORD_WHILE) {
        return this->parseWhile();
    } else if(current.type == KEYWORD_DO){
        return this->parseDoWhile();
    } 
    else if(current.type == KEYWORD_SWITCH) {
        return this->parseSwitch();
    }
    // Check for case/default labels
    else if(current.type == KEYWORD_CASE) {
        return this->parseCaseLabel();
    } else if(current.type == KEYWORD_DEFAULT) {
        return this->parseDefaultLabel();
    }
    // Check for jump statements
    else if(current.type == KEYWORD_RETURN) {
        return this->parseReturn();
    } else if(current.type == KEYWORD_CONTINUE) {
        return this->parseContinue();
    } else if(current.type == KEYWORD_BREAK) {
        return this->parseBreak();
    } else if(current.type == KEYWORD_GOTO) {
        return this->parseGoto();
    }
    else if(current.type == ID && tokens[currentPos+1].type == OP_COLON){
        return this->parseLabel();
    }
    // Check for structured types
    else if(current.type == KEYWORD_STRUCT) {
        return this->parseStruct();
    } else if(current.type == KEYWORD_ENUM) {
        return this->parseEnum();
    } else if(current.type == KEYWORD_UNION) {
        return this->parseUnion();
    } else if(current.type == KEYWORD_TYPEDEF) {
        return this->parseTypedef();
    } 
    // Check if it's a declaration (starts with type/storage class)
    else if(isThisTokenDataTypeOrPropToken(current)) {
        return this->parseCurrentDecl();
    } else {
        // Otherwise it's an expression statement
        return this->parseExpressionStatement();
    }
}

DeclarationNode* Parser::parseCurrentDecl(){
    // use if else to find the best parser for current node
    if(isThisTokenDataTypeOrPropToken(this->tokens[this->currentPos])){ // found some data type or prop
        // call the parser fucntion with data type as first token
        this->parseDataTypeFoundDeclaration();
        // grab the last node that was pushed to allAST and return it
        if(!allAST.empty()){
            ASTNode* last = allAST.back();
            allAST.pop_back();
            return dynamic_cast<DeclarationNode*>(last);
        }
        return nullptr;
    } 
    return nullptr;
}

bool Parser::isThisTokenDataTypeOrPropToken(Token currToken){
    switch(currToken.type){
        // data type
        case KEYWORD_INT:
        case KEYWORD_CHAR:  
        case KEYWORD_BOOL:
        case KEYWORD_FLOAT:
        case KEYWORD_DOUBLE:
        case KEYWORD_VOID:

        // sie modifier
        case KEYWORD_SHORT:
        case KEYWORD_LONG:

        // sign modifier
        case KEYWORD_SIGNED:
        case KEYWORD_UNSIGNED:

        // type qualifiers
        case KEYWORD_CONST:
        case KEYWORD_VOLATILE:
        case KEYWORD_RESTRICT:

        // storage class
        case KEYWORD_STATIC:
        case KEYWORD_EXTERN:
        case KEYWORD_AUTO:
        case KEYWORD_REGISTER:        
            return true;
        default:
            return false;
    }
    
}

bool Parser::isThisTokenDataBaseTypeToken(Token currToken){
    switch(currToken.type){
        // data type
        case KEYWORD_INT:
        case KEYWORD_CHAR:  
        case KEYWORD_BOOL:
        case KEYWORD_FLOAT:
        case KEYWORD_DOUBLE:
        case KEYWORD_VOID:         
            return true;
        default:
            return false;
    }
}

bool Parser::isThisTokenSignModifierToken(Token currToken){
    switch(currToken.type){
        // sign modifier
        case KEYWORD_SIGNED:
        case KEYWORD_UNSIGNED:       
            return true;
        default:
            return false;
    }
}

bool Parser::isThisTokenSizeModifierToken(Token currToken){
    switch(currToken.type){
        // sie modifier
        case KEYWORD_SHORT:
        case KEYWORD_LONG:       
            return true;
        default:
            return false;
    }
}

bool Parser::isThisTokenTypeQualifierToken(Token currToken){
    switch(currToken.type){
        // type qualifiers
        case KEYWORD_CONST:
        case KEYWORD_VOLATILE:
        case KEYWORD_RESTRICT:       
            return true;
        default:            return false;
    }
}

bool Parser::isThisTokenStorageClassToken(Token currToken){
    switch(currToken.type){
        // storage class
        case KEYWORD_STATIC:
        case KEYWORD_EXTERN:
        case KEYWORD_AUTO:
        case KEYWORD_REGISTER:        
            return true;
        default:
            return false;
    }
}

bool Parser::isThisTokenStructUnionEnumToken(Token currToken){
    switch(currToken.type){
        case KEYWORD_STRUCT:
        case KEYWORD_UNION:
        case KEYWORD_ENUM:
            return true;
        default:
            return false;
    }
}

bool  Parser::isThisStringPresentAsKeyInTrHm(string key){
    // check if the given string is present as a key in type registry hashmap
    auto search = this->typeRegisry.find(key);
    if(search != this->typeRegisry.end()){
        return true; // found
    }
    return false; // not found
}

bool Parser::isThisStringPresentAsKeyInTdMap(string key){
    // check if the given string is present as a key in typedef hashmap
    auto search = this->tdMap.find(key);
    if(search != this->tdMap.end()){
        return true; // found
    }
    return false; // not found
}

bool Parser::isCurrentIdValidTdAlias(){
    // check if the current Id is valid TD alias in that specific position or just a ID that is variable/function name
    // if next token is ; [ = , :  then return false

    Token nextToken = this->tokens[this->currentPos + 1];
    switch(nextToken.type){
        case SEMICOLON:
        case LBRACKET:
        case OP_ASSIGN:
        case COMMA:
        case OP_COLON:
            return false; // just an ID 
        default:
            return true; // valid TD alias
    }
    

}

Token Parser::getCurrentToken(){
    if(this->currentPos < this->tokens.size()){
        return this->tokens[this->currentPos];
    } else{
        // return an invalid token if out of bounds
        return Token{ERROR, "", -1, -1};
    }
}

DeclarationNode* Parser::parseDataTypeFoundDeclaration(){    

    DeclarationNode* temp;

    // generate an object to store the current data type
    dataTypeHolder currType = dataTypeHolder(*this);

    // collect the data type and props
    currType.getDataType();

    // validate this data type (0 if valid for both, 1 if valid ONLY for var , 2 if valid ONLY for func , -1 if invalid)
    short retCode = currType.isCurrentTypeValid();
    if(retCode == -1){
        cout << "Type is invalid\n";
        exit(1);
    }
    // now, the type decl is valid

    varNameHolder currName = varNameHolder(*this);
    
    if(retCode == 2){ // valid ONLY for func (void*)
        temp = currName.getVarName(currType , false);
        this->allAST.push_back(temp);
        
        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;
        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
    } else if(retCode == 1){ // valid ONLY for var
        multiDecl:
        temp = currName.getVarName(currType , false);
        
        this->allAST.push_back(temp);

        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;
        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
    } else{ // can be valid for both var or func
        // lookup algo to check if it is var or func decl
        
        // if(var) proceed var decl        
        temp = currName.getVarName(currType , false);
        this->allAST.push_back(temp);
        
        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;
        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
        
        // if(func) proceed func decl
    }


    // decide if this is a fucntion decl or varibale decl
    
    // if function, check if defined and proceed accordingly
    // function part finished here

    // if variable
    
    // check if initialized

    // check if multi-declratations

    // if multi-call , use same data type combination we got for other variable declaraitons also    
    

    // generate all the AST and return accordignly
    return nullptr; // nodes stored in this->allAST
}

StatementNode* Parser::parseExpressionStatement() {
    // Parse the expression
    ExpressionNode* expr = parseExpression(0, false, -1);
    
    // Expect semicolon
    if(this->tokens[this->currentPos].type != SEMICOLON) {
        cout << "Expected ; after expression statement\n";
        exit(1);
    }
    this->currentPos++; // skip ;
    
    // Create and return ExpressionStatementNode
    // Caller is responsible for adding to appropriate collection
    return new ExpressionStatementNode(expr);
}

// Control flow statements
StatementNode* Parser::parseIf() {
    
    currentPos++; // skip keyword if

    if(tokens[currentPos].type != LPAREN){
        cout << "Expected opening ( of if condition\n";
        exit(1);
    }

    currentPos++; // skip (

    ExpressionNode* cond = parseExpression(0 , false , 1);

    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of if condition\n";
        exit(1);
    }

    currentPos++; // skip )

    vector<ASTNode*> ifStatements; // to store 1 statment if not block

    BlockExpressionNode* ifBlock = nullptr; // block if condition is true 

    if(tokens[currentPos].type == LBRACE){ // block
        ifBlock = parseBlock(*this);
    } else{ // single line
        ifStatements.push_back(startParsingOfCurrentToken());
        ifBlock = new BlockExpressionNode(ifStatements);
    }

    if(tokens[currentPos].type == KEYWORD_ELSE){
        

        currentPos++; // skip keyword else

        BlockExpressionNode* elseBlock;

        if(tokens[currentPos].type == LBRACE){
            elseBlock = parseBlock(*this);
        } else{
            vector<ASTNode*> elseStatements;
            elseStatements.push_back(startParsingOfCurrentToken());
            elseBlock = new BlockExpressionNode(elseStatements);
        }

        return new IfStatementNode(cond , ifBlock , true , elseBlock);

    } else{
        return new IfStatementNode(cond , ifBlock , false , nullptr);
    }

}

StatementNode* Parser::parseFor() {    

    currentPos++; // skip keyword for

    if(tokens[currentPos].type != LPAREN){
        cout << "Expected ( after for\n";
        exit(1);
    }

    currentPos++; // skip (

    ASTNode* init = nullptr;

    if(tokens[currentPos].type != SEMICOLON){ // if there is some initialization statement
        if(isThisTokenDataTypeOrPropToken(tokens[currentPos]) || 
           (tokens[currentPos].type == ID && isThisStringPresentAsKeyInTdMap(tokens[currentPos].data))){
            init = parseCurrentDecl(); // consumes ;
        } else {
            init = parseExpressionStatement(); // consumes ;
        }
    } else{
        currentPos++; // skip ; (empty init)
    }        

    ExpressionNode* cond = nullptr;

    if(tokens[currentPos].type != SEMICOLON){ // if there is some condition
        cond = parseExpression(0 , false , -1);
    }

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after for condition\n";
        exit(1);
    }

    currentPos++; // skip ;

    ExpressionNode* incr = nullptr;

    if(tokens[currentPos + 1].type != SEMICOLON){ // if there is some increment
        incr = parseExpression(0 , false , 1);
    } 

    if(tokens[currentPos].type != RPAREN){
        cout << "Expected ) after for increment\n";
        exit(1);
    }

    currentPos++; // skip )

    if(tokens[currentPos].type == LBRACE){
        BlockExpressionNode* forBlock = parseBlock(*this);
        return new ForStatementNode(init , cond , incr , forBlock);
    } else{
        vector<ASTNode*> forStatements;
        forStatements.push_back(startParsingOfCurrentToken());
        BlockExpressionNode* forBlock = new BlockExpressionNode(forStatements);
        return new ForStatementNode(init , cond , incr , forBlock);
    }

    

}

StatementNode* Parser::parseWhile() {
    currentPos++; // skip keyword while

    if(tokens[currentPos].type != LPAREN){
        cout << "Expected opening ( of while condition\n";
        exit(1);
    }

    currentPos++; // skip (

    ExpressionNode* cond = parseExpression(0 , false , 1);

    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of while condition\n";
        exit(1);
    }

    currentPos++; // skip )

    vector<ASTNode*> whileStatements; // to store 1 statment if not block

    BlockExpressionNode* whileBlock = nullptr; // block if condition is true 

    if(tokens[currentPos].type == LBRACE){ // block
        whileBlock = parseBlock(*this);
    } else{ // single line
        whileStatements.push_back(startParsingOfCurrentToken());
        whileBlock = new BlockExpressionNode(whileStatements);
    }

    return new WhileStatementNode(cond , whileBlock);

    
}

StatementNode* Parser::parseDoWhile() {
    
    currentPos++; // skip keyword do

    BlockExpressionNode* doWhileBlock = nullptr; 

    if(tokens[currentPos].type == LBRACE){
        doWhileBlock = parseBlock(*this);
    } else{
        vector<ASTNode*> doWhileStatements; 
        doWhileStatements.push_back(startParsingOfCurrentToken());
        doWhileBlock = new BlockExpressionNode(doWhileStatements);
    }

    if(tokens[currentPos].type != KEYWORD_WHILE){
        cout << "Expected while after do-while block\n";
        exit(1);        
    }

    currentPos++; // skip keyword while

    if(tokens[currentPos].type != LPAREN){
        cout << "Expected ( after while in do-while\n";
        exit(1);
    }

    currentPos++; // skip (

    ExpressionNode* cond = parseExpression(0 , false , 1);

    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of do-while condition\n";
        exit(1);
    }

    currentPos++; // skip )

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after do-while condition\n";
        exit(1);
    }

    currentPos++; // skip ;

    return new DoWhileStatementNode(cond , doWhileBlock);

}

StatementNode* Parser::parseSwitch() {
    
    currentPos++; // skip keyword switch

    if(tokens[currentPos].type != LPAREN){
        cout << "Expected opening ( of switch condition\n";
        exit(1);
    }

    currentPos++; // skip (

    ExpressionNode* cond = parseExpression(0 , false , 1);

    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of switch condition\n";
        exit(1);
    }

    currentPos++; // skip )

    BlockExpressionNode* switchBlock = nullptr;

    if(tokens[currentPos].type == LBRACE){ // block
        switchBlock = parseBlock(*this);
    } else{
        cout << "Expected { after switch condition\n";
        exit(1);
    }

    return new SwitchStatementNode(cond , switchBlock);

}

StatementNode* Parser::parseCaseLabel() {
    currentPos++; // skip keyword case

    if(currentPos >= tokens.size()){
        cout << "Expected expression after 'case'\n";
        exit(1);
    }

    ExpressionNode* caseExpr = parseExpression(0, false, 4);

    if(currentPos >= tokens.size() || tokens[currentPos].type != OP_COLON){
        cout << "Expected ':' after case expression\n";
        exit(1);
    }

    currentPos++; // skip :

    return new CaseLabelNode(caseExpr);
}

StatementNode* Parser::parseDefaultLabel() {
    currentPos++; // skip keyword default

    if(currentPos >= tokens.size() || tokens[currentPos].type != OP_COLON){
        cout << "Expected ':' after 'default'\n";
        exit(1);
    }

    currentPos++; // skip :

    return new DefaultLabelNode();
}

// Structured types
DeclarationNode* Parser::parseStruct() {
    cout << "struct parsing isn't implemented yet\n";
    exit(1);
    return nullptr;
}

DeclarationNode* Parser::parseEnum() {
    cout << "enum parsing isn't implemented yet\n";
    exit(1);
    return nullptr;
}

DeclarationNode* Parser::parseUnion() {
    cout << "union parsing isn't implemented yet\n";
    exit(1);
    return nullptr;
}

DeclarationNode* Parser::parseTypedef() {
    cout << "typedef parsing isn't implemented yet\n";
    exit(1);
    return nullptr;
}

// Jump statements
StatementNode* Parser::parseReturn() {
    currentPos++; // skip keyword return

    ExpressionNode* retExpr = nullptr;

    if(tokens[currentPos].type != SEMICOLON){
        retExpr = parseExpression(0, false, -1);
    }

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after return statement\n";
        exit(1);
    }

    currentPos++; // skip ;

    return new ReturnStatementNode(retExpr);
}

StatementNode* Parser::parseContinue() {
    currentPos++; // skip keyword continue

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after continue\n";
        exit(1);
    }

    currentPos++; // skip ;

    return new ContinueStatementNode();
}

StatementNode* Parser::parseBreak() {
    currentPos++; // skip keyword break

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after break\n";
        exit(1);
    }

    currentPos++; // skip ;

    return new BreakStatementNode();
}

StatementNode* Parser::parseGoto() {
    // Skip 'goto' keyword
    currentPos++;
    
    // Next token must be identifier
    if(currentPos >= tokens.size() || tokens[currentPos].type != ID) {
        cout << "Expected identifier after 'goto'\n";
        exit(1);
    }
    
    // Create identifier node
    ExpressionNode* labelName = new IdentifierNode(tokens[currentPos].data);
    currentPos++;
    
    // Expect semicolon
    if(currentPos >= tokens.size() || tokens[currentPos].type != SEMICOLON) {
        cout << "Expected ';' after goto statement\n";
        exit(1);
    }
    currentPos++;
    
    // Create and return goto statement node
    return new GotoStatementNode(labelName);
}
    
StatementNode* Parser::parseLabel() {
    // Current token should be identifier
    if(currentPos >= tokens.size() || tokens[currentPos].type != ID) {
        cout << "Expected identifier for label\n";
        exit(1);
    }
    
    // Create identifier node
    ExpressionNode* labelName = new IdentifierNode(tokens[currentPos].data);
    currentPos++;
    
    // Expect colon
    if(currentPos >= tokens.size() || tokens[currentPos].type != OP_COLON) {
        cout << "Expected ':' after label\n";
        exit(1);
    }
    currentPos++;
    
    // Create and return label statement node
    return new LabelStatementNode(labelName);
}



