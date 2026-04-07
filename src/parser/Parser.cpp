#include "Parser.h"
#include <vector>
#include "Helper.h"
#include <iostream>

#include <cstdint>

using namespace std;

// Define the global myStack variable
vector<locationStack> myStack;

vector<ASTNode*> tempASTStorage;

bool typedDefTracker = false;

long long unsigned anonTracker = 0;

string* anonTagNameGen(){
    string* retVal = new string("ANON_TAG_" + to_string(anonTracker));
    anonTracker++;
    return retVal;
}

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

    // generate all relavent AST
    // keep parsing untill tokens are finished
    while(this->currentPos < tokens.size()){
        // ASTNode* node = startParsingOfCurrentToken();
        ASTNode** node = highLevelParse();

        // add nodes in node to allast
        for(int i = 0; node != nullptr && node[i] != nullptr; i++){
            allAST.push_back(node[i]);
        }

        
        
        // if(node != nullptr) {
        //     // Check if it's a statement
        //     StatementNode* stmt = dynamic_cast<StatementNode*>(node);
        //     if(stmt != nullptr) {
        //         allStmts.push_back(stmt);
        //     }
        //     // Check if it's a declaration
        //     DeclarationNode* decl = dynamic_cast<DeclarationNode*>(node);
        //     if(decl != nullptr) {
        //         allDeclNodes.push_back(decl);
        //     }
        // }
    }

    // generating program node containing an array of all decl nodes
    // ProgramNode* myRootNode = new ProgramNode(allDeclNodes);
    ProgramNode* myRootNode = new ProgramNode(allAST);

    // returning the program node as root node
    return myRootNode;
}

ASTNode** Parser::highLevelParse(){
    Token current = this->tokens[this->currentPos];
    

    if(current.type == KEYWORD_TYPEDEF){ // typedef always comes as first word if it is typedef
        return parseTypedef();
        
    } else if(isThisTokenDataTypeOrPropToken(current)){ // for global var decl, func decl, froward decl, extern 
        itIsVarDeclInstead:                
        return parseDataTypeFoundDeclaration();
    } else if(current.type == KEYWORD_STRUCT || current.type == KEYWORD_UNION || current.type == KEYWORD_ENUM){ // covers parts of struct/enum/union without any data type prop before them

        cout << "High level struct/union/enum found\n";

        if(this->tokens[this->currentPos+1].type == ID && this->tokens[this->currentPos+2].type != SEMICOLON && this->tokens[this->currentPos+2].type != LBRACE){ // it is just var decl, and not struct definition, switch to data type decl parsing
            goto itIsVarDeclInstead;
        }

        if(current.type == KEYWORD_STRUCT){            
            return parseStruct(nullptr);            
        }
    } else if(current.type == ID){ // can be related to typedef or type registry, might need to check
        // now, we need to check if this id can be part of typedef alias or not, if yes, then need to use lookup to decide how to proceed
        
        if(isCurrentIdValidTdAlias()){ 
            // it has to be td alias if exist
            

            if(isThisStringPresentAsKeyInTdMap(current.data)){
                // it is present as key in td map


                goto itIsVarDeclInstead;
            }else{
                cout << "Error: ID " << current.data << " found at high level parsing, but it is not a valid typedef alias\n";
                exit(1);
            }
        } else{
            cout << "Unknown identifier found at high level parsing: " << current.data << "\n";
            exit(1);
        }
        
    }
    else{ 
        cout << "high level unknown token found\n";
        cout << "Token data: " << current.data << "\n";

        exit(1);
    }

    return nullptr;
}

ASTNode** Parser::startParsingOfCurrentToken() {
    Token current = this->tokens[this->currentPos];
    
    // Check for control flow statements
    if(current.type == KEYWORD_IF) {
        StatementNode* ans = this->parseIf();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_FOR) {
        StatementNode* ans = this->parseFor();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_WHILE) {
        StatementNode* ans = this->parseWhile();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_DO){
        StatementNode* ans = this->parseDoWhile();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } 
    else if(current.type == KEYWORD_SWITCH) {
        StatementNode* ans = this->parseSwitch();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for case/default labels
    else if(current.type == KEYWORD_CASE) {
        StatementNode* ans = this->parseCaseLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_DEFAULT) {
        StatementNode* ans = this->parseDefaultLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for jump statements
    else if(current.type == KEYWORD_RETURN) {
        StatementNode* ans = this->parseReturn();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_CONTINUE) {
        StatementNode* ans = this->parseContinue();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_BREAK) {
        StatementNode* ans = this->parseBreak();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_GOTO) {
        StatementNode* ans = this->parseGoto();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    else if(current.type == ID && tokens[currentPos+1].type == OP_COLON){
        StatementNode* ans = this->parseLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for structured types
    else if(current.type == KEYWORD_STRUCT) {
        // Only call parseStruct if this appears to be a struct definition (has LBRACE)
        // Check: next token must be ID, and token after that could be LBRACE for definition
        if(currentPos + 2 < tokens.size() &&
           tokens[currentPos + 1].type == ID &&
           tokens[currentPos + 2].type == LBRACE) {
            // This is a struct definition
            ASTNode** myHelper = this->parseStruct(nullptr);
            return myHelper;
        }
        // If not a definition, fall through to data type parsing
        goto itIsVarDeclInstead;
    } 
    else if(current.type == KEYWORD_ENUM) {
    //     return this->parseEnum();
    // } else if(current.type == KEYWORD_UNION) {
    //     return this->parseUnion();
    // } else if(current.type == KEYWORD_TYPEDEF) {
    //     return this->parseTypedef();
    } 
    // Check if it's a declaration (starts with type/storage class)
    else if(isThisTokenDataTypeOrPropToken(current)) {
        itIsVarDeclInstead:
        return parseDataTypeFoundDeclaration();
    } else {
        // Otherwise it's an expression statement
        StatementNode* ans = this->parseExpressionStatement();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }

    return nullptr; // Should never reach here

}

DeclarationNode* Parser::parseCurrentDecl(){
    // use if else to find the best parser for current node
    // if(isThisTokenDataTypeOrPropToken(this->tokens[this->currentPos])){ // found some data type or prop
        // call the parser fucntion with data type as first token
        this->parseDataTypeFoundDeclaration();
        // grab the last node that was pushed to allAST and return it
        if(!allAST.empty()){
            ASTNode* last = allAST.back();
            allAST.pop_back();
            return dynamic_cast<DeclarationNode*>(last);
        }
    // } 
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

ASTNode** Parser::parseDataTypeFoundDeclaration(){

    DeclarationNode* temp;

    // vector<DeclarationNode*> list; // to store all decl nodes (can be multiple in case of multi-decl)
    vector<ASTNode*> list; // to store all decl nodes (can be multiple in case of multi-decl)

    // generate an object to store the current data type
    dataTypeHolder currType = dataTypeHolder(*this);

    // collect the data type and props
    int retValueDecl = currType.getDataType();

    // if(retValueDecl == 2){ // struct defintion, alr added to ast, dont add again
    //     return nullptr;
    // }

    if(retValueDecl == 2){ // struct definition found, need to call the struct definition func with proper token 
        // return nullptr; 



        // convert tempASTStorage to arrya and push all to list
        for(uint64_t i=0 ; i<tempASTStorage.size(); i++){
            list.push_back(tempASTStorage[i]);
        }

        tempASTStorage.clear();


        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array

        return arr; // nodes stored in this->allAST

        /*
        
            this is not done yet, needs to fix this properly, need to find some way to call parse struct or something from here while at the same time keeping track of data type and prop used 

            UPDATE: parseStruct is happened but not added properly to it's parent block 

        */

    }

    // validate this data type (0 if valid for both, 1 if valid ONLY for var , 2 if valid ONLY for func , -1 if invalid)

    // short retCode = 0;

    short retCode = currType.isCurrentTypeValid();



    

    if(retCode == -1){
        cout << "Type is invalid\n";
        exit(1);
    }
    // now, the type decl is valid

    varNameHolder currName = varNameHolder(*this);
    
    if(retCode == 2){ // valid ONLY for func (void)
        temp = currName.getVarName(currType , false);

        list.push_back(temp);

        // this->allAST.push_back(temp);
        
        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;
        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
    } else if(retCode == 1){ // valid ONLY for var
        multiDecl:
        temp = currName.getVarName(currType , false);
        
        // this->allAST.push_back(temp);

        list.push_back(temp);


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
        // this->allAST.push_back(temp);

        list.push_back(temp);
        
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

    ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
    for(uint64_t i = 0; i < list.size(); i++){
        arr[i] = list[i];
    }
    arr[list.size()] = nullptr; // null terminate the arraya

    return arr; // nodes stored in this->allAST
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
        ifStatements.push_back(startParsingOfCurrentToken()[0]);
        ifBlock = new BlockExpressionNode(ifStatements);
    }

    if(tokens[currentPos].type == KEYWORD_ELSE){
        

        currentPos++; // skip keyword else

        BlockExpressionNode* elseBlock;

        if(tokens[currentPos].type == LBRACE){
            elseBlock = parseBlock(*this);
        } else{
            vector<ASTNode*> elseStatements;
            elseStatements.push_back(startParsingOfCurrentToken()[0]);
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
        forStatements.push_back(startParsingOfCurrentToken()[0]);
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
        whileStatements.push_back(startParsingOfCurrentToken()[0]);
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
        doWhileStatements.push_back(startParsingOfCurrentToken()[0]);
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
ASTNode** Parser::parseStruct(dataTypeHolder* helperDeclName) {
    
    currentPos++; // skip keyword struct

    bool tagNameExist = false;
    Token tagName;

    string* anonTagName = nullptr;

    vector<ASTNode*> list; // list to store struct decl node and maybe var decl nodes also if exist

    if(tokens[currentPos].type == ID){ // tagName exist
        tagNameExist = true;
        tagName = tokens[currentPos]; // save tagName in token
        currentPos++; // skip tagName
    } else{
        // if tagName doesnt exist, create a anonymous tagname for this [will be used for the validaiton later, but will not be exposed]

        anonTagName = anonTagNameGen();
        
    }

    if(tokens[currentPos].type == SEMICOLON){ // can be forward decl if tagName exists        
        if(!tagNameExist){
            cout << "Expected tagName for forward decl\n";
            exit(1);
        }

        if(typedDefTracker){ // if typedef is there, then either ID is exepceted or definition is expeceted
            cout << "typedef struct forward decl NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;

        this->typeRegisry[tagName.data] = "struct"; // adding in type registry to add forw decl


        ForwardDeclarationNode* fwdDecl = new ForwardDeclarationNode(KEYWORD_STRUCT , tagName.data);
        
        list.push_back(fwdDecl);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array


        return arr;

        // this->allAST.push_back(fwdDecl);
        // return fwdDecl; // return for startParsing to collect
        
    } else if(tokens[currentPos].type == ID){        

        if(typedDefTracker){ 
            // it is typedef , not var decl
        } else{
            // it is var decl
        }

    }

    

    if(tokens[currentPos].type != LBRACE){
        /*
            this may be sruct var decl or error, but definitely not struct definition.

            switch the control flow to the var decl side
        */

        currentPos -= 2;

        return nullptr; // indicating the need to shift the control flow properly
        
        cout << "struct decl found, not implemented yet\n";
        exit(1);
    }

    // if the control flow reaches here, it is struct definition.

    BlockExpressionNode* structBlock = nullptr;

    // this->typeRegisry[tagName.data] = "struct"; // might need tor revert it if the further code fails, take care !!!!!!!!!!!

    if(tagNameExist){
        this->typeRegisry[tagName.data] = "struct";
    } else{
        this->typeRegisry[*anonTagName] = "struct";
    }

    // cout << "Block parsing starting at " << this->tokens[this->currentPos-1].data << "\n";
    structBlock = parseBlock(*this); // parse the block



    if(tokens[currentPos].type == SEMICOLON){
        if(!tagNameExist){
            cout << "Expected atleast one out of tagName or varName for struct definition\n";
            exit(1);
        }

        if(typedDefTracker){
            cout << "typedef struct definition without typedef name NOT allowed\n";
            exit(1);
        }

        /*

            now, since no var is declared wiht it, it cna NOT have any specifier like extern/volatile/restrict/const/static

            check the data type prop for this and reject if any of them exists

        */

        

        if(helperDeclName != nullptr){
            cout << "var is NOT decl , hence extern/volatile/restrict/const/static NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;

        StructDefinitionNode* structDef;

        if(tagNameExist){
            structDef = new StructDefinitionNode(tagNameExist , tagName.data , structBlock);
        } else{
            structDef = new StructDefinitionNode(tagNameExist , *anonTagName , structBlock);
        }

        

        /*
            need to add the current new struct definition in the TR
        */

        list.push_back(structDef);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the arraya

        return arr;

        // this->allAST.push_back(structDef);
        // return structDef; // return for startParsing to collect

    }

    // now there is some varName also afetr the }

    StructDefinitionNode* structDef;

    if(tagNameExist){
        structDef = new StructDefinitionNode(tagNameExist , tagName.data , structBlock);
    } else{
        structDef = new StructDefinitionNode(tagNameExist , *anonTagName , structBlock);
    }       

    list.push_back(structDef);

    
    // this->allAST.push_back(structDef);

    /*
        if it came from dataTypeDecl, it has some specifiers like extern/volatile/restrict/const/static , we here need to validate it and the manually add struct tagName to it in the dataType object
    */

    // validity of data type prop array is STILL not checked so far

    // helperDeclName validaiton algo    
    if(helperDeclName){

        {
            if(helperDeclName->signModifiersArray.size() != 0){ // signed/unsigned NOT allowed
                cout << "Error in sign\n";
                exit(1);
            }

            if(helperDeclName->sizeModifiersArray.size() != 0){ // size modifiers NOT allowed
                cout << "Error in size\n";
                exit(1);
            }

            if(helperDeclName->storageClassArray.size() > 1){ // multiple storage class NOT allowed
                cout << "Error is storage class size\n";
                exit(1);        
            } else if(helperDeclName->storageClassArray.size() == 1){ // if there is a storage class, it should not be auto or register
                if(helperDeclName->storageClassArray.back() == KEYWORD_AUTO || helperDeclName->storageClassArray.back() == KEYWORD_REGISTER){
                    cout << "auto/register NOT allowed\n";
                    exit(1);
                }                
            }

            if(helperDeclName->typeQualifiersArray.size() > 3){ // max 3 type qualifiers allowed
                cout << "Error in type qualifier\n";
                exit(1);
            } else if(helperDeclName->typeQualifiersArray.size() > 1){ // if there are more than 1 type qualifiers, they should not be same
                for(long unsigned i=0 ; i<helperDeclName->typeQualifiersArray.size() ; i++){
                    for(long unsigned j=0 ; j<helperDeclName->typeQualifiersArray.size() ; j++){
                        if(i == j) continue;

                        if(helperDeclName->typeQualifiersArray[i] == helperDeclName->typeQualifiersArray[j]){
                            cout << "Dublicate type qualifiers NOT allowed\n";
                            exit(1);
                        }
                    }
                }
            }

            /*
                this validation is pending rn, that restrict always require * with the var
                
                might add this in semantic phase
            */

            
        }



        // add this as base type is NOT present 
        helperDeclName->trKeywordArray.push_back(KEYWORD_STRUCT);
        helperDeclName->trBaseArray.push_back(tagName.data);
    }

    
    
    
    

    getVarAgain:


    /*
    
        so, now the var decl also exist with the struct definition
        now, it is needed to check the data type prop object to check it's validaity for this
    
    */


    varNameHolder* structVarName = new varNameHolder(*this);

    if(!helperDeclName){
        helperDeclName = new dataTypeHolder(*this);

        // helperDeclName->baseTypeArray.push_back(HELPER_TOKEN);
        helperDeclName->trKeywordArray.push_back(KEYWORD_STRUCT);

        // Generate automatic name for anonymous structs if needed
        // if(tagName.data.empty()){
        //     static int anonStructCounter = 0;
        //     tagName.data = "__anon_struct_" + to_string(anonStructCounter++);
        // }
        // helperDeclName->trBaseArray.push_back(tagName.data);
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }

    // dataTypeHolder* helper = nullptr;

    list.push_back(structVarName->getVarName(*helperDeclName, false));

    // this->allDeclNodes.push_back(structVarName->getVarName(*helperDeclName, false));


    // this->allAST.push_back(new DeclarationNode(helperDeclName , structVarName));

    // generate struct decl node

    if(tokens[currentPos].type == COMMA){        

        currentPos++; // skipp ,
        goto getVarAgain;
    }

    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; here to close it\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return struct definition node for startParsing to collect
    
    ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
    for(uint64_t i = 0; i < list.size(); i++){
        arr[i] = list[i];
    }
    arr[list.size()] = nullptr; // null terminate the arraya

    return arr;
    // return structDef;
    
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

ASTNode** Parser::parseTypedef() {
    

    typedDefTracker = true;

    currentPos++; // skip typedef keyword

    vector<ASTNode*> list; // list to store struct decl node and maybe var decl nodes also if exist

    int tempIndexHolder = currentPos;

    dataTypeHolder* original = new dataTypeHolder(*this);

    int retValueDecl = original->getDataType();

    original->isCurrentTypeValid();


    if(retValueDecl == 2){ // struct definition 
        
        // convert tempASTStorage to arrya and push all to list
        for(uint64_t i=0 ; i<tempASTStorage.size(); i++){
            // list.push_back(tempASTStorage[i]);

            /*
                here,
                    it will contain 1 struct definition ast
                    it can also contain variabel decl nodes, so need to convert them accoridngly to the typedef decl
            */

            if(i == 0){
                list.push_back(tempASTStorage[i]); // add the struct to the list as it is the foundation for typedef                
            } else{

                
                VariableDeclarationNode* parentObj = dynamic_cast<VariableDeclarationNode*>(tempASTStorage[i]);

                varNameHolder* alias = &parentObj->varName; // store the varName object of the (so called) var decl, coz need to acces this to generate typedef ast 

                tdMapPair* rhs = new tdMapPair;
                rhs->nameProp = alias; // store the varName string in the tdMapPair

                short helperStorageInt = tempIndexHolder; 

                // below code is copy pasted to specially deal with struct
                {
                    addAgain2:

                    // first add the data type prop as string in sequence
                    while(isThisTokenDataTypeOrPropToken(tokens[tempIndexHolder])){
                        rhs->declProp.push_back(tokens[tempIndexHolder].data);
                    
                        tempIndexHolder++;
                    }
                
                    // check if next one is some typedef of original parsing is done
                    if(tokens[tempIndexHolder].type == KEYWORD_STRUCT){
                        // this is pending for now
                    
                        rhs->declProp.push_back(tokens[tempIndexHolder].data);
                    
                        tempIndexHolder++; // skip struct keyword
                    
                        if(tokens[tempIndexHolder].type == ID){ // struct tagName also exist
                            rhs->declProp.push_back(tokens[tempIndexHolder].data);
                        
                            tempIndexHolder++; // skip struct tagName
                        } else{
                            // it is anon struct def with typedef, need to give it a name myself

                            if(StructDefinitionNode* structDef = dynamic_cast<StructDefinitionNode*>(list.back())) {
                                rhs->declProp.push_back(structDef->tagName);
                            }
                        }
                    

                    }
                
                    if(tokens[tempIndexHolder].type == ID){
                        if(tokens[tempIndexHolder+1].type == SEMICOLON){
                            // it is the situation of normal typedef usecase (no complex expr in alias)
                        
                            // generate typedef ast
                        }
                    
                        if(isThisStringPresentAsKeyInTdMap(tokens[tempIndexHolder].data)){
                            rhs->declProp.push_back(tokens[tempIndexHolder].data);
                        
                            tempIndexHolder++; // skip ID
                        
                            goto addAgain2;
                        }
                    }
                                        
                    
                } 
                
                
                

                list.push_back(new TypedefDeclarationNode(rhs->declProp , rhs->nameProp));

                if(i != tempASTStorage.size() - 1){
                    tempIndexHolder = helperStorageInt; 
                    continue;
                }
                        
                ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
                for(uint64_t i = 0; i < list.size(); i++){
                    arr[i] = list[i];
                }
                arr[list.size()] = nullptr; // null terminate the arraya
            
                // tdMap[rhs->nameProp->namePropArray[0].varName].push_back(rhs); 
            
                string typedefAliasName = rhs->nameProp->namePropArray[0].varName;
                tdMap[typedefAliasName].push_back(*rhs); 
            
                tempASTStorage.clear();            


                // cout << "Current token is : " << tokens[currentPos].data << "\n";

                if(tokens[currentPos].type == SEMICOLON) currentPos++; // skip ;
                
            
                return arr;

            }

            

        }
        

        // String 

        // tempASTStorage.clear();


        // ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        // for(uint64_t i = 0; i < list.size(); i++){
        //     arr[i] = list[i];
        // }
        // arr[list.size()] = nullptr; // null terminate the array

        // return arr; // nodes stored in this->allAST

    }

     // validation ????

    bool isFirstDeclator = true; // Track if this is the first declarator in multi-declarator typedef
    short indexSaver = tempIndexHolder; // Save index before looping through data type tokens

    multiTypeDefDeclLabel: // Label for handling multiple declarators

    varNameHolder* alias = new varNameHolder(*this);

    // For subsequent declarators, force reset of static variables in getVarName()
    DeclarationNode* parentObj = alias->getVarName(*original, false, !isFirstDeclator);

    tdMapPair* rhs = new tdMapPair;

    if(VariableDeclarationNode* varObj = dynamic_cast<VariableDeclarationNode*>(parentObj)){ // using dynamic case to access the child instance varaibles properly
        rhs->nameProp = &varObj->varName;
    } else if(FunctionDeclarationNode* funcObj = dynamic_cast<FunctionDeclarationNode*>(parentObj)){
        rhs->nameProp = &funcObj->funcName;
    }



    {
        addAgain:

        // first add the data type prop as string in sequence
        while(isThisTokenDataTypeOrPropToken(tokens[tempIndexHolder])){
            rhs->declProp.push_back(tokens[tempIndexHolder].data);

            tempIndexHolder++;
        }

        // check if next one is some typedef of original parsing is done
        if(tokens[tempIndexHolder].type == KEYWORD_STRUCT){

            // this is pending for now

            rhs->declProp.push_back(tokens[tempIndexHolder].data);

            tempIndexHolder++; // skip struct keyword

            if(tokens[tempIndexHolder].type == ID){ // struct tagName also exist
                rhs->declProp.push_back(tokens[tempIndexHolder].data);

                tempIndexHolder++; // skip struct tagName
            } else{
                // it is anon struct def with typedef, need to give it a name myself

                // get the anon struct name from the latest struct definition

                if(StructDefinitionNode* structDef = dynamic_cast<StructDefinitionNode*>(list.back())) {
                    rhs->declProp.push_back(structDef->tagName);
                }
            }

            
        }

        if(tokens[tempIndexHolder].type == ID){
            if(tokens[tempIndexHolder+1].type == SEMICOLON){
                // it is the situation of normal typedef usecase (no complex expr in alias)

                // generate typedef ast
            }

            if(isThisStringPresentAsKeyInTdMap(tokens[tempIndexHolder].data)){
                rhs->declProp.push_back(tokens[tempIndexHolder].data);

                tempIndexHolder++; // skip ID

                goto addAgain;
            }
        }
    }

    // generate typedef ast

    // Add the current declarator's typedef to the list and map
    list.push_back(new TypedefDeclarationNode(rhs->declProp , rhs->nameProp));

    string typedefAliasName = rhs->nameProp->namePropArray[0].varName;
    tdMap[typedefAliasName].push_back(*rhs);

    // Check if there are more declarators
    if(tokens[currentPos].type == COMMA){
        // Multiple declarators in this typedef

        // Reset for next declarator in multiple typedef declaration
        alias->resetDataTypeAndNameObjectForNext(*original);

        tempIndexHolder = indexSaver; // Reset tempIndexHolder to reprocess data type tokens for next declarator
        currentPos++; // skip comma

        isFirstDeclator = false; // Mark that we're on a subsequent declarator
        goto multiTypeDefDeclLabel; // Go parse the next declarator
    } else if(tokens[currentPos].type == SEMICOLON){
        // End of typedef declaration
        currentPos++; // skip ;

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array

        return arr;
    } else{
        cout << "Expected ; or , after typedef declarator\n";
        exit(1);
    }
    // return new TypedefDeclarationNode(rhs->declProp , alias);

    

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



