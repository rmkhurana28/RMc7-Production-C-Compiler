#include "Parser.h"
#include <vector>
#include "Helper.h"
#include <iostream>

#include <cstdint>

using namespace std;

// Define the global myStack variable
vector<locationStack> myStack;

// this is ast storage, used to add ast to the array in some special edge cases
vector<ASTNode*> tempASTStorage;

// bool that tells if current one is typedef tracker or not
bool typedDefTracker = false;

// counter to keep track of number of anonymous names generated so far,
long long unsigned anonTracker = 0;

varNameHolder* tempVarNameHolder = nullptr;

// typedef hashmap
unordered_map<string, vector<tdMapPair>> tdMap;

// func generating anon name whenever called
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
    
    while(this->currentPos < tokens.size()){ // keep parsing untill tokens are finished
        ASTNode** node = highLevelParse();

        // add nodes in node to allast
        for(int i = 0; node != nullptr && node[i] != nullptr; i++){
            allAST.push_back(node[i]);
        }
                
    }

    // generating program node containing an array of all decl nodes    
    ProgramNode* myRootNode = new ProgramNode(allAST);

    // returning the program node as root node
    return myRootNode;
}

ASTNode** Parser::highLevelParse(){ // entire parsing algo sequence-control is implemented here
    
    // setting current to the current token
    Token current = this->tokens[this->currentPos]; 
    

    if(current.type == KEYWORD_TYPEDEF){ // typedef always comes as first word if it is typedef
        return parseTypedef(); 
        
    } else if(isThisTokenDataTypeOrPropToken(current)){ // for global var decl, func decl, froward decl, extern 
        itIsVarDeclInstead: // came from struct/enum/union when that keyword corresponds to the var decl and not their own definition or came from td map alias name
        return parseDataTypeFoundDeclaration();

    } else if(current.type == KEYWORD_STRUCT || current.type == KEYWORD_UNION || current.type == KEYWORD_ENUM){ // covers parts of struct/enum/union without any data type prop before them

        if(this->tokens[this->currentPos+1].type == ID && this->tokens[this->currentPos+2].type != SEMICOLON && this->tokens[this->currentPos+2].type != LBRACE){ // it is just var decl, and not struct/enum/union definition, switch to data type decl parsing
            goto itIsVarDeclInstead; 
        }

        if(current.type == KEYWORD_STRUCT){ // parse struct
            return parseStruct(nullptr);
        } else if(current.type == KEYWORD_UNION){ // parse union
            return parseUnion(nullptr);
        } else if(current.type == KEYWORD_ENUM){ // parse enum
            return parseEnum(nullptr);
        }
    } else if(current.type == ID){ // can be related to typedef or type registry, might need to check
        
        // now, we need to check if this id can be part of typedef alias or not, if yes, then need to use lookup to decide how to proceed
        
        if(isCurrentIdValidTdAlias()){  // checking if it is actually acting as a tdAlias by using look-up algo or if it is just another ID
            // it has to be td alias if exist

            if(isThisStringPresentAsKeyInTdMap(current.data)){ // checking if ID is presetnas key in typedef hashmap
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
    else{ // unknown token found, not present in td Alias also
        cout << "high level unknown token found\n";
        cout << "Token data: " << current.data << "\n";

        exit(1);
    }

    return nullptr;
}

ASTNode** Parser::startParsingOfCurrentToken() { // entire if-else sequence control is implemented here for current token

    Token current = this->tokens[this->currentPos]; // set current to the current token properly
    
    // Check for control flow statements
    if(current.type == KEYWORD_IF) { // parse if
        StatementNode* ans = this->parseIf();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_FOR) { // parse for
        StatementNode* ans = this->parseFor();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_WHILE) { // parse while
        StatementNode* ans = this->parseWhile();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_DO){ // parse do-while
        StatementNode* ans = this->parseDoWhile();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } 
    else if(current.type == KEYWORD_SWITCH) { // parse switch
        StatementNode* ans = this->parseSwitch();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for case/default labels
    else if(current.type == KEYWORD_CASE) { // parse case
        StatementNode* ans = this->parseCaseLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_DEFAULT) { // parse default
        StatementNode* ans = this->parseDefaultLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for jump statements
    else if(current.type == KEYWORD_RETURN) { // parse return
        StatementNode* ans = this->parseReturn();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_CONTINUE) { // parse continue
        StatementNode* ans = this->parseContinue();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_BREAK) { // parse break
        StatementNode* ans = this->parseBreak();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    } else if(current.type == KEYWORD_GOTO) { // parse goto
        StatementNode* ans = this->parseGoto();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    else if(current.type == ID && tokens[currentPos+1].type == OP_COLON){ // parse label
        StatementNode* ans = this->parseLabel();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }
    // Check for structured types
    else if(current.type == KEYWORD_STRUCT) { // parse struct definition (this covers inside a block, not global one)
        // Only call parseStruct if this appears to be a struct definition (has LBRACE)
        // Check: next token must be ID, and token after that must be LBRACE for definition
        if(currentPos + 2 < tokens.size() && tokens[currentPos + 1].type == ID && tokens[currentPos + 2].type == LBRACE) {

            // This is a struct definition
            ASTNode** myHelper = this->parseStruct(nullptr);
            
            return myHelper;
        }

        // If not a definition, fall through to data type parsing
        goto itIsVarDeclInstead;
    }
    else if(current.type == KEYWORD_UNION) { // parse union definiton (this covers inside a block, not global one)
        // Only call parseUnion if this appears to be a union definition (has LBRACE)
        // Check: next token must be ID, and token after that musta be LBRACE for definition
        if(currentPos + 2 < tokens.size() && tokens[currentPos + 1].type == ID && tokens[currentPos + 2].type == LBRACE) {

            // This is a union definition
            ASTNode** myHelper = this->parseUnion(nullptr);

            return myHelper;
        }
        
        // If not a definition, fall through to data type parsing
        goto itIsVarDeclInstead;
    }
    else if(current.type == KEYWORD_ENUM) { // parse enum definition (this covers inside a block, not global one)
        if(currentPos + 2 < tokens.size() && tokens[currentPos+1].type == ID && tokens[currentPos+2].type == LBRACE) {

            // This is an enum definition
            ASTNode** myHelper = this->parseEnum(nullptr);

            return myHelper;
        }

        // If not a definition, fall through to data type parsing
        goto itIsVarDeclInstead;
    }
    
    // Check if it's a declaration (starts with type/storage/signed/short/long)
    else if(isThisTokenDataTypeOrPropToken(current)) { // data type or it's any prop
        itIsVarDeclInstead:
        return parseDataTypeFoundDeclaration();
    } else if(current.type == ID && isThisStringPresentAsKeyInTdMap(current.data)){ 
        // it is valid td alias, so it has to be declaration
        goto itIsVarDeclInstead;
    } else if(current.type == LBRACE){ // parse block

        // it is a block statement, can be part of function definition or control flow statement, but we will parse it as block statement here and later decide how to use it based on the parent node
        ASTNode* ans = parseBlock(*this);
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;

        return dummy;
        
    }
    else {
        // Otherwise it's an expression statement
        StatementNode* ans = this->parseExpressionStatement();
        ASTNode** dummy = new ASTNode*[2];
        dummy[0] = ans;
        dummy[1] = nullptr;
        return dummy;
    }

    return nullptr; // Should never reach here

}


/*
    dont know if below func is being used somehwere, it was made in the early design, and then later refactoring of algo was done, need to properly see if this func is being used or if it is safe to remove from here.
*/
DeclarationNode* Parser::parseCurrentDecl(){ 
    // use if else to find the best parser for current node
    // if(isThisTokenDataTypeOrPropToken(this->tokens[this->currentPos])){ // found some data type or prop
        // call the parser fucntion with data type as first token
        ASTNode** result = this->parseDataTypeFoundDeclaration();
        // grab the first node from the returned array
        if(result != nullptr && result[0] != nullptr){
            return dynamic_cast<DeclarationNode*>(result[0]);
        }
    // }
    return nullptr;
}

// checking if token is either any data type or any of it's property
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

// checking if token is any of the base data type 
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

// checking if token is any of the sign modifier
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

// checking if token is any of the size modifier
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

// checking if token is any of the type qualifier
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

// checking if token is any of the storage class prop
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

// checking if token is any of the struct/enum/union token
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

// checking if token is present as key in the type registry hashmap
bool  Parser::isThisStringPresentAsKeyInTrHm(string key){
    // check if the given string is present as a key in type registry hashmap
    auto search = this->typeRegisry.find(key);
    if(search != this->typeRegisry.end()){
        return true; // found
    }
    return false; // not found
}

// checking if token is present as alias in the typedef hashmap
bool Parser::isThisStringPresentAsKeyInTdMap(string key){
    // check if the given string is present as a key in typedef hashmap
    auto search = tdMap.find(key);
    if(search != tdMap.end()){
        return true; // found
    }
    return false; // not found
}

// checking if token is actually a tdAlias or just an id
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

// returning curent token
Token Parser::getCurrentToken(){
    if(this->currentPos < this->tokens.size()){
        return this->tokens[this->currentPos];
    } else{
        // return an invalid token if out of bounds
        return Token{ERROR, "", -1, -1};
    }
}

// func to parse when the current token found is either a data type or any of it's property, basically variable/func/others decl 
ASTNode** Parser::parseDataTypeFoundDeclaration(){
    
    DeclarationNode* temp;
    
    vector<ASTNode*> list; // to store all decl nodes (can be multiple in case of multi-decl)

    // generate an object to store the current data type
    dataTypeHolder currType = dataTypeHolder(*this);

    // collect the data type and props
    int retValueDecl = currType.getDataType();

    // if(retValueDecl == 2){ // struct defintion, alr added to ast, dont add again
    //     return nullptr;
    // }

    if(retValueDecl == 2){ // struct/union definition found, need to call the struct definition func with proper token         

        /*
            so, now since the return value if 2, it means it is some struct/union/enum definition
            so, before returning 2 from that func, it is alr parsed, and stored in the global var, "tempASTStorage"
            so, now, need to copy the ast from "tempASTStorage" to "list"
        */

        // convert tempASTStorage to array and push all to "list"
        for(uint64_t i=0 ; i<tempASTStorage.size(); i++){
            list.push_back(tempASTStorage[i]);
        }

        tempASTStorage.clear(); // clearing global storage now since everything is copied alr


        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array

        return arr; // nodes stored in this->allAST        

    }

    // validate this data type (0 if valid for both, 1 if valid ONLY for var , 2 if valid ONLY for func , -1 if invalid)    
    short retCode = currType.isCurrentTypeValid();

    if(retCode == -1){ // data type is completely invalid
        cout << "Type is invalid\n";
        exit(1);
    }
    
    // now, the type decl is valid

    // create name holder object
    varNameHolder currName = varNameHolder(*this);

    if(retCode == 2){ // valid ONLY for func (void)

        // parse the func name
        temp = currName.getVarName(currType , false, true);

        
        if(tempVarNameHolder != nullptr){ // what is happening here? 


            // uint64_t orig = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.size();

            {
                uint64_t tempVarNameSize = tempVarNameHolder->namePropArray.size();

                if(tempVarNameSize != 1){
                    for(uint64_t i=1 ; i<tempVarNameSize ; i++){
                        static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.push_back(tempVarNameHolder->namePropArray[i]);
                    }
                }
            }


            // it had some td expansion logic, need to modify the varName accordingly

            // varNameHolder* modifiedVarName = new varNameHolder(*tempVarNameHolder); // copy the tempVarNameHolder to the new modified one

            // modifiedVarName->namePropArray[0] = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[0];

            // if(orig != 1){
            //     for(uint64_t i=1 ; i<orig ; i++){
            //         // modifiedVarName->namePropArray.push_back(temp->varName.namePropArray[i]);
            //         modifiedVarName->namePropArray.push_back(static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[i]);
            //     }
            // }

            // static_cast<VariableDeclarationNode*>(temp)->varName = *modifiedVarName; // assign the modified var name back to temp

            tempVarNameHolder = nullptr; 
            
        }

        // push temp to the "list"
        list.push_back(temp);        
        
        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;

        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line ended
            this->currentPos++;
        }
    } else if(retCode == 1){ // valid ONLY for var
        multiDecl:

        // parse var name
        temp = currName.getVarName(currType , false, true);        

        if(tempVarNameHolder != nullptr){ // what is happening here again?


            // uint64_t orig = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.size();

            {
                uint64_t tempVarNameSize = tempVarNameHolder->namePropArray.size();

                if(tempVarNameSize != 1){
                    for(uint64_t i=1 ; i<tempVarNameSize ; i++){
                        static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.push_back(tempVarNameHolder->namePropArray[i]);
                    }
                }
            }


            // it had some td expansion logic, need to modify the varName accordingly

            // varNameHolder* modifiedVarName = new varNameHolder(*tempVarNameHolder); // copy the tempVarNameHolder to the new modified one

            // modifiedVarName->namePropArray[0] = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[0];
            

            // if(orig != 1){
            //     for(uint64_t i=1 ; i<orig ; i++){
            //         // modifiedVarName->namePropArray.push_back(temp->varName.namePropArray[i]);
            //         modifiedVarName->namePropArray.push_back(static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[i]);
            //     }
            // }

            

            // static_cast<VariableDeclarationNode*>(temp)->varName = *modifiedVarName; // assign the modified var name back to temp

            tempVarNameHolder = nullptr; 
            
        }

        // push temp to "list"
        list.push_back(temp);

        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;

        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
    } else{ // can be valid for both var or func

        // might need lookup algo to check if it is var or func decl ???

        /*
            IIRC, getVarName itself checks if it is var or func decl/definition, so no need to use seperate lookup algo here to check, it is alr done and it return the ast accordignly

            but but but, if this is the case, then why make 3 different cases? func only? var only? func/var? ???
        */     
        
        // parse name
        temp = currName.getVarName(currType , false, true);        

        if(tempVarNameHolder != nullptr){ // again, what is going on here ???


            // store the original size of varName array before modification
            // uint64_t orig = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.size();

            {
                uint64_t tempVarNameSize = tempVarNameHolder->namePropArray.size();

                if(tempVarNameSize != 1){
                    for(uint64_t i=1 ; i<tempVarNameSize ; i++){
                        static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray.push_back(tempVarNameHolder->namePropArray[i]);
                    }
                }
            }            


            // it had some td expansion logic, need to modify the varName accordingly

            // varNameHolder* modifiedVarName = new varNameHolder(*tempVarNameHolder); // copy the tempVarNameHolder to the new modified one

            // modifiedVarName->namePropArray[0] = static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[0];

            // if(orig != 1){
            //     for(uint64_t i=1 ; i<orig ; i++){
            //         // modifiedVarName->namePropArray.push_back(temp->varName.namePropArray[i]);
            //         modifiedVarName->namePropArray.push_back(static_cast<VariableDeclarationNode*>(temp)->varName.namePropArray[i]);
            //     }
            // }

            // static_cast<VariableDeclarationNode*>(temp)->varName = *modifiedVarName; // assign the modified var name back to temp

            tempVarNameHolder = nullptr; 
            
        }

        // push temp to "list"
        list.push_back(temp);
        
        if(this->tokens[this->currentPos].type == COMMA){ // multiple decl
            this->currentPos++;
            goto multiDecl;

        } else if(this->tokens[this->currentPos].type == SEMICOLON){ // line closed
            this->currentPos++;
        }
        
    }
       
    

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
    // Caller is responsible for adding to appropriate collection/parent
    return new ExpressionStatementNode(expr);
}

// Control flow statements
// parsing if
StatementNode* Parser::parseIf() {
    
    currentPos++; // skip keyword if

    // ( expected after if keyword
    if(tokens[currentPos].type != LPAREN){ 
        cout << "Expected opening ( of if condition\n";
        exit(1);
    }

    currentPos++; // skip (

    // parse the expression inside if condition
    ExpressionNode* cond = parseExpression(0 , false , 1); // sending 1 as we need to stop when ) is countered

    // ) expected after cond of if
    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of if condition\n";
        exit(1);
    }

    currentPos++; // skip )

    vector<ASTNode*> ifStatements; // to store 1 statment if not block

    BlockExpressionNode* ifBlock = nullptr; // block if condition is true 

    if(tokens[currentPos].type == LBRACE){ // there is block of statements inside if
        ifBlock = parseBlock(*this); // parse block
    } else{ // single line statement inside if
        ifStatements.push_back(startParsingOfCurrentToken()[0]); // parse the single statement
        ifBlock = new BlockExpressionNode(ifStatements); // make block ast containing only 1 ast 
    }

    // check if "else" present after "if"
    if(tokens[currentPos].type == KEYWORD_ELSE){        

        currentPos++; // skip keyword else

        // else block decl
        BlockExpressionNode* elseBlock;

        if(tokens[currentPos].type == LBRACE){ // there is block of statements inside else
            elseBlock = parseBlock(*this); // parse block
        } else{ // single line statement inside else
            vector<ASTNode*> elseStatements; 
            elseStatements.push_back(startParsingOfCurrentToken()[0]); // parse the single statement
            elseBlock = new BlockExpressionNode(elseStatements); // make block ast containing only 1 ast 
        }

        return new IfStatementNode(cond , ifBlock , true , elseBlock); // returning if-else ast

    } else{
        return new IfStatementNode(cond , ifBlock , false , nullptr); // returning if ast
    }

}

// parsing for
StatementNode* Parser::parseFor() {    

    currentPos++; // skip keyword for

    // ( expected after if keyword
    if(tokens[currentPos].type != LPAREN){
        cout << "Expected ( after for\n";
        exit(1);
    }

    currentPos++; // skip (

    // decl to store init part of for 
    ASTNode* init = nullptr;

    if(tokens[currentPos].type != SEMICOLON){ // if there is some initialization statement
        if(isThisTokenDataTypeOrPropToken(tokens[currentPos]) || 
           (tokens[currentPos].type == ID && isThisStringPresentAsKeyInTdMap(tokens[currentPos].data))){ // checking if it is some kind of decl
            // below , need to see if i can replcae "parseCurrentDecl()" with other func, since it is not used anywhere else
            init = parseCurrentDecl(); // parse current decl (consumes ;)
        } else { // it is just expression and not decl
            init = parseExpressionStatement(); // parse expression (consumes ;)
        }
    } else{
        currentPos++; // skip ; (empty init)
    }        

    // decl to store cond part of for 
    ExpressionNode* cond = nullptr;

    if(tokens[currentPos].type != SEMICOLON){ // if there is some condition
        cond = parseExpression(0 , false , -1); // parse the cond, can ONLY be expr, not decl
    }

    // ; expected after cond part
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after for condition\n";
        exit(1);
    }

    currentPos++; // skip ;

    // decl to store incr part of for 
    ExpressionNode* incr = nullptr;

    if(tokens[currentPos + 1].type != SEMICOLON){ // if there is some increment
        // parse the cond, can ONLY be expr, not decl
        incr = parseExpression(0 , false , 1); // sending 1 as we need to stop when ) is countered
    } 

    // ) expected after incr
    if(tokens[currentPos].type != RPAREN){
        cout << "Expected ) after for increment\n";
        exit(1);
    }

    currentPos++; // skip )

    if(tokens[currentPos].type == LBRACE){ // there is a block of statements under for 
        BlockExpressionNode* forBlock = parseBlock(*this); // parse block
        return new ForStatementNode(init , cond , incr , forBlock); // return for ast
    } else{ // just one statement under for
        vector<ASTNode*> forStatements;
        forStatements.push_back(startParsingOfCurrentToken()[0]); // parse 1 statement
        BlockExpressionNode* forBlock = new BlockExpressionNode(forStatements); // generate block containing only 1 statement
        return new ForStatementNode(init , cond , incr , forBlock); // return for ast
    }    

}

// parsing while
StatementNode* Parser::parseWhile() {
    currentPos++; // skip keyword while

    // ( expected after while keyword
    if(tokens[currentPos].type != LPAREN){
        cout << "Expected opening ( of while condition\n";
        exit(1);
    }

    currentPos++; // skip (

    // parse the expression inside while condition
    ExpressionNode* cond = parseExpression(0 , false , 1); // sending 1 as we need to stop when ) is countered

    // ) expected after cond of while
    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of while condition\n";
        exit(1);
    }

    currentPos++; // skip )

    vector<ASTNode*> whileStatements; // to store 1 statment if not block

    BlockExpressionNode* whileBlock = nullptr; // block if condition is true

    // parse while body - either block or single statement
    if(tokens[currentPos].type == LBRACE){ // there is block of statements inside while
        whileBlock = parseBlock(*this); // parse block
    } else{ // single line statement inside while
        whileStatements.push_back(startParsingOfCurrentToken()[0]); // parse the single statement
        whileBlock = new BlockExpressionNode(whileStatements); // make block ast containing only 1 ast
    }

    return new WhileStatementNode(cond , whileBlock); // returning while ast


}

// parsing do-while
StatementNode* Parser::parseDoWhile() {

    currentPos++; // skip keyword do

    BlockExpressionNode* doWhileBlock = nullptr;

    // parse do-while body - either block or single statement
    if(tokens[currentPos].type == LBRACE){ // there is block of statements inside do-while
        doWhileBlock = parseBlock(*this); // parse block
    } else{ // single line statement inside do-while
        vector<ASTNode*> doWhileStatements;
        doWhileStatements.push_back(startParsingOfCurrentToken()[0]); // parse the single statement
        doWhileBlock = new BlockExpressionNode(doWhileStatements); // make block ast containing only 1 ast
    }

    // while keyword is mandatory after do block in do-while construct
    if(tokens[currentPos].type != KEYWORD_WHILE){
        cout << "Expected while after do-while block\n";
        exit(1);
    }

    currentPos++; // skip keyword while

    // ( expected after while in do-while
    if(tokens[currentPos].type != LPAREN){
        cout << "Expected ( after while in do-while\n";
        exit(1);
    }

    currentPos++; // skip (

    // parse the condition expression inside while of do-while
    ExpressionNode* cond = parseExpression(0 , false , 1); // sending 1 as we need to stop when ) is countered

    // ) expected after cond of do-while
    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of do-while condition\n";
        exit(1);
    }

    currentPos++; // skip )

    // ; expected after do-while condition, this is required to complete the syntax
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after do-while condition\n";
        exit(1);
    }

    currentPos++; // skip ;

    return new DoWhileStatementNode(cond , doWhileBlock); // returning do-while ast

}

// parsing switch
StatementNode* Parser::parseSwitch() {

    currentPos++; // skip keyword switch

    // ( expected after switch keyword
    if(tokens[currentPos].type != LPAREN){
        cout << "Expected opening ( of switch condition\n";
        exit(1);
    }

    currentPos++; // skip (

    // parse the expression inside switch condition
    ExpressionNode* cond = parseExpression(0 , false , 1); // sending 1 as we need to stop when ) is countered

    // ) expected after cond of switch
    if(tokens[currentPos].type != RPAREN){
        cout << "Expected closing ) of switch condition\n";
        exit(1);
    }

    currentPos++; // skip )

    BlockExpressionNode* switchBlock = nullptr;

    // switch body MUST be a block (unlike if/while), parse it directly
    if(tokens[currentPos].type == LBRACE){ // block containing case/default labels
        switchBlock = parseBlock(*this); // parse the switch block
    } else{
        cout << "Expected { after switch condition\n";
        exit(1);
    }

    return new SwitchStatementNode(cond , switchBlock); // returning switch ast

}

// parsing case label
StatementNode* Parser::parseCaseLabel() {
    currentPos++; // skip keyword case

    // bounds check to ensure there is expression after case keyword
    if(currentPos >= tokens.size()){
        cout << "Expected expression after 'case'\n";
        exit(1);
    }

    // parse the case constant expression
    ExpressionNode* caseExpr = parseExpression(0, false, 4); // sending 4 to stop at : (colon is delimiter for case label)

    // : (colon) is mandatory after case expression to complete case label syntax
    if(currentPos >= tokens.size() || tokens[currentPos].type != OP_COLON){
        cout << "Expected ':' after case expression\n";
        exit(1);
    }

    currentPos++; // skip :

    return new CaseLabelNode(caseExpr); // returning case label ast
}

// parsin default label
StatementNode* Parser::parseDefaultLabel() {
    currentPos++; // skip keyword default

    // : (colon) is mandatory after default keyword to complete default label syntax
    if(currentPos >= tokens.size() || tokens[currentPos].type != OP_COLON){
        cout << "Expected ':' after 'default'\n";
        exit(1);
    }

    currentPos++; // skip :

    return new DefaultLabelNode(); // returning default label ast
}

// parsing struct
ASTNode** Parser::parseStruct(dataTypeHolder* helperDeclName) {
    
    currentPos++; // skip keyword struct

    // helper to keep track if tagName is found or not
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

        // tagname doesnt exist and ; found after "struct" keyword, hence error
        if(!tagNameExist){
            cout << "Expected tagName for forward decl\n";
            exit(1);
        }

        /*
            typeDefTraker is true when the current ast parsing that is being done involves typedef keyword (typedef definition)
        */

        if(typedDefTracker){ // if typedef is there, then either ID is expected or definition is expeceted
            cout << "typedef struct forward decl NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;
        
        this->typeRegisry[tagName.data] = "struct"; // adding in type registry to add forw decl

        // generate forward declaration node and push to "list"
        ForwardDeclarationNode* fwdDecl = new ForwardDeclarationNode(KEYWORD_STRUCT , tagName.data);
        
        // push to "list"
        list.push_back(fwdDecl);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array

        return arr; 
        
    } else if(tokens[currentPos].type == ID){ // ID found after struct or tagname (if after tagname, has to be var decl)      

        if(typedDefTracker){ 
            // it is typedef , not var decl
        } else{
            // it is var decl
        }

    }

    

    if(tokens[currentPos].type != LBRACE){
        /*
            this may be sruct var decl or error, but definitely not struct definition.
            hence, switch the control flow to the var decl side
        */

        currentPos -= 2; // reset currentPost

        return nullptr; // indicating the need to shift the control flow properly        
    }

    // if the control flow reaches here, it is struct definition.

    // block to store struct definitions
    BlockExpressionNode* structBlock = nullptr;

    // this->typeRegisry[tagName.data] = "struct"; // might need tor revert it if the further code fails, take care !!!!!!!!!!!

    if(tagNameExist){ // if tagname exist, attach tagname to "struct" in type registry
        this->typeRegisry[tagName.data] = "struct";
    } else{ // if tagname doest exist, attach anonTagName to "struct" in type registry
        this->typeRegisry[*anonTagName] = "struct";
    }
    
    structBlock = parseBlock(*this); // parse the block

    // semicolon found after struct block, means no var decl is there, and if it is typedef decl, then its error since alias name is expected after struct block in typedef case
    if(tokens[currentPos].type == SEMICOLON){

        // need to have atleast one out of tagName or varName for definition
        if(!tagNameExist){
            cout << "Expected atleast one out of tagName or varName for struct definition\n";
            exit(1);
        }

        // alias expected in case of typedef definition
        if(typedDefTracker){
            cout << "typedef struct definition without typedef name NOT allowed\n";
            exit(1);
        }

        /*

            now, since no var is declared wiht it, it can NOT have any specifier like extern/volatile/restrict/const/static
            check the data type prop for this and reject if any of them exists

        */
    
        // variable isnt declared, hence no prop specifier is allowed
        if(helperDeclName != nullptr){
            cout << "var is NOT decl , hence extern/volatile/restrict/const/static NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;
        
        StructDefinitionNode* structDef;

        if(tagNameExist){ // actual tagName struct definition ast
            structDef = new StructDefinitionNode(tagNameExist , tagName.data , structBlock);
        } else{ // anonTagName sruct definition ast
            structDef = new StructDefinitionNode(tagNameExist , *anonTagName , structBlock);
        }        

        // push struct definition ast to "list"
        list.push_back(structDef);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the arraya

        return arr;       

    }

    /*
        now there is some varName also after block of struct definition
    */ 

    
    StructDefinitionNode* structDef;

    // generate struct definition ast first
    if(tagNameExist){ // actual tagName struct definition ast
        structDef = new StructDefinitionNode(tagNameExist , tagName.data , structBlock);
    } else{ // anonTagName sruct definition ast
        structDef = new StructDefinitionNode(tagNameExist , *anonTagName , structBlock);
    }       

    // push struct definition node to "list"
    list.push_back(structDef);    

    /*
        if it came from dataTypeDecl, it has some specifiers like extern/volatile/restrict/const/static , we here need to validate it and the manually add struct tagName to it in the dataType object
    */

    // validity of data type prop array is STILL not checked so far

    // helperDeclName validaiton algo    
    if(helperDeclName){ // validate if exist
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
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }        

    getVarAgain:

    /*
        so, now var decl also exist with struct definition
        if data type prop object was alr present, it is alr validated in code just before this comment, so now if it exists,we r good to go, if it doesnt exists, we need to create it here manually for the var decl
    */    


    // struct var decl
    varNameHolder* structVarName = new varNameHolder(*this);

    // if data type prop doesnt exists, create it
    if(!helperDeclName){

        // new data type prop object
        helperDeclName = new dataTypeHolder(*this);
        
        // manually add keyword "struct" and it's corresponding base name to the newly created dataType prop object
        helperDeclName->trKeywordArray.push_back(KEYWORD_STRUCT);        
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }    

    // push var decl ast to "list"
    list.push_back(structVarName->getVarName(*helperDeclName, false));            

    // if , is present, it means, there is multi var decl
    if(tokens[currentPos].type == COMMA){        

        currentPos++; // skipp ,
        goto getVarAgain; // make another ast for multi var decl
    }

    // ; expected 
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
}

// parsing enum
ASTNode** Parser::parseEnum(dataTypeHolder* helperDeclName) {

    currentPos++; // skip keyword enum

    // helper to keep track if tagName is found or not
    bool tagNameExist = false;
    Token tagName;

    string* anonTagName = nullptr;

    vector<ASTNode*> list; // list to store enum decl node and maybe var decl nodes also if exist

    if(tokens[currentPos].type == ID){ // tagName exist
        tagNameExist = true;
        tagName = tokens[currentPos]; // save tagName in token
        currentPos++; // skip tagName
    } else{
        // if tagName doesnt exist, create a anonymous tagname for this
        anonTagName = anonTagNameGen();
    }

    // Enum forward declarations are NOT allowed - must have definition
    if(tokens[currentPos].type != LBRACE){
        /*
            this may be enum var decl or error, but definitely not enum definition.
            hence, switch the control flow to the var decl side
        */

        currentPos -= 2; // reset currentPost

        return nullptr; // indicating the need to shift the control flow properly
    }

    // if the control flow reaches here, it is enum definition.

    // block to store enum definitions
    EnumBlockExpressionNode* enumBlock = nullptr;

    if(tagNameExist){ // if tagname exist, attach tagname to "enum" in type registry
        this->typeRegisry[tagName.data] = "enum";
    } else{ // if tagname doest exist, attach anonTagName to "enum" in type registry
        this->typeRegisry[*anonTagName] = "enum";
    }

    enumBlock = parseEnumBlock(*this); // parse the enum block

    // semicolon found after enum block, means no var decl is there, and if it is typedef decl, then its error since alias name is expected after enum block in typedef case
    if(tokens[currentPos].type == SEMICOLON){

        // need to have atleast one out of tagName or varName for definition
        if(!tagNameExist){
            cout << "Expected atleast one out of tagName or varName for enum definition\n";
            exit(1);
        }

        // alias expected in case of typedef definition
        if(typedDefTracker){
            cout << "typedef enum definition without typedef name NOT allowed\n";
            exit(1);
        }

        // variable isnt declared, hence no prop specifier is allowed
        if(helperDeclName != nullptr){
            cout << "var is NOT decl , hence extern/volatile/restrict/const/static NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;

        EnumDefinitionNode* enumDef;

        if(tagNameExist){ // actual tagName enum definition ast
            enumDef = new EnumDefinitionNode(tagNameExist , tagName.data , enumBlock);
        } else{ // anonTagName enum definition ast
            enumDef = new EnumDefinitionNode(tagNameExist , *anonTagName , enumBlock);
        }

        // push enum definition ast to "list"
        list.push_back(enumDef);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the arraya

        return arr;
    }

    /*
        now there is some varName also after block of enum definition
    */

    EnumDefinitionNode* enumDef;

    // generate enum definition ast first
    if(tagNameExist){ // actual tagName enum definition ast
        enumDef = new EnumDefinitionNode(tagNameExist , tagName.data , enumBlock);
    } else{ // anonTagName enum definition ast
        enumDef = new EnumDefinitionNode(tagNameExist , *anonTagName , enumBlock);
    }

    // push enum definition node to "list"
    list.push_back(enumDef);

    /*
        if it came from dataTypeDecl, it has some specifiers like extern/volatile/restrict/const/static , we here need to validate it and the manually add enum tagName to it in the dataType object
    */

    // validity of data type prop array is STILL not checked so far

    // helperDeclName validaiton algo
    if(helperDeclName){ // validate if exist
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
        helperDeclName->trKeywordArray.push_back(KEYWORD_ENUM);
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }

    getEnumVarAgain:

    /*
        so, now var decl also exist with enum definition
        if data type prop object was alr present, it is alr validated in code just before this comment, so now if it exists,we r good to go, if it doesnt exists, we need to create it here manually for the var decl
    */

    // enum var decl
    varNameHolder* enumVarName = new varNameHolder(*this);

    // if data type prop doesnt exists, create it
    if(!helperDeclName){

        // new data type prop object
        helperDeclName = new dataTypeHolder(*this);

        // manually add keyword "enum" and it's corresponding base name to the newly created dataType prop object
        helperDeclName->trKeywordArray.push_back(KEYWORD_ENUM);
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }

    // push var decl ast to "list"
    list.push_back(enumVarName->getVarName(*helperDeclName, false));

    // if , is present, it means, there is multi var decl
    if(tokens[currentPos].type == COMMA){

        currentPos++; // skip ,
        goto getEnumVarAgain; // make another ast for multi var decl
    }

    // ; expected
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; here to close it\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return enum definition node for startParsing to collect
    ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
    for(uint64_t i = 0; i < list.size(); i++){
        arr[i] = list[i];
    }
    arr[list.size()] = nullptr; // null terminate the arraya

    return arr;
}

// parsing union
ASTNode** Parser::parseUnion(dataTypeHolder* helperDeclName) {

    currentPos++; // skip keyword union

    // helper to keep track if tagName is found or not
    bool tagNameExist = false;
    Token tagName;

    string* anonTagName = nullptr;

    vector<ASTNode*> list; // list to store union decl node and maybe var decl nodes also if exist

    if(tokens[currentPos].type == ID){ // tagName exist
        tagNameExist = true;
        tagName = tokens[currentPos]; // save tagName in token
        currentPos++; // skip tagName
    } else{
        // if tagName doesnt exist, create a anonymous tagname for this [will be used for the validaiton later, but will not be exposed]
        anonTagName = anonTagNameGen();

    }

    if(tokens[currentPos].type == SEMICOLON){ // can be forward decl if tagName exists

        // tagname doesnt exist and ; found after "union" keyword, hence error
        if(!tagNameExist){
            cout << "Expected tagName for forward decl\n";
            exit(1);
        }

        /*
            typeDefTraker is true when the current ast parsing that is being done involves typedef keyword (typedef definition)
        */

        if(typedDefTracker){ // if typedef is there, then either ID is expected or definition is expeceted
            cout << "typedef union forward decl NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;

        this->typeRegisry[tagName.data] = "union"; // adding in type registry to add forw decl

        // generate forward declaration node and push to "list"
        ForwardDeclarationNode* fwdDecl = new ForwardDeclarationNode(KEYWORD_UNION , tagName.data);

        // push to "list"
        list.push_back(fwdDecl);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the array

        return arr;

    } else if(tokens[currentPos].type == ID){ // ID found after union or tagname (if after tagname, has to be var decl)

        if(typedDefTracker){
            // it is typedef , not var decl
        } else{
            // it is var decl
        }

    }



    if(tokens[currentPos].type != LBRACE){
        /*
            this may be union var decl or error, but definitely not union definition.
            hence, switch the control flow to the var decl side
        */

        currentPos -= 2; // reset currentPost

        return nullptr; // indicating the need to shift the control flow properly
    }

    // if the control flow reaches here, it is union definition.

    // block to store union definitions
    BlockExpressionNode* unionBlock = nullptr;

    // this->typeRegisry[tagName.data] = "union"; // might need tor revert it if the further code fails, take care !!!!!!!!!!!

    if(tagNameExist){ // if tagname exist, attach tagname to "union" in type registry
        this->typeRegisry[tagName.data] = "union";
    } else{ // if tagname doest exist, attach anonTagName to "union" in type registry
        this->typeRegisry[*anonTagName] = "union";
    }

    unionBlock = parseBlock(*this); // parse the block

    // semicolon found after union block, means no var decl is there, and if it is typedef decl, then its error since alias name is expected after union block in typedef case
    if(tokens[currentPos].type == SEMICOLON){

        // need to have atleast one out of tagName or varName for definition
        if(!tagNameExist){
            cout << "Expected atleast one out of tagName or varName for union definition\n";
            exit(1);
        }

        // alias expected in case of typedef definition
        if(typedDefTracker){
            cout << "typedef union definition without typedef name NOT allowed\n";
            exit(1);
        }

        /*

            now, since no var is declared wiht it, it can NOT have any specifier like extern/volatile/restrict/const/static
            check the data type prop for this and reject if any of them exists

        */

        // variable isnt declared, hence no prop specifier is allowed
        if(helperDeclName != nullptr){
            cout << "var is NOT decl , hence extern/volatile/restrict/const/static NOT allowed\n";
            exit(1);
        }

        currentPos++; // skip ;

        UnionDefinitionNode* unionDef;

        if(tagNameExist){ // actual tagName union definition ast
            unionDef = new UnionDefinitionNode(tagNameExist , tagName.data , unionBlock);
        } else{ // anonTagName union definition ast
            unionDef = new UnionDefinitionNode(tagNameExist , *anonTagName , unionBlock);
        }

        // push union definition ast to "list"
        list.push_back(unionDef);

        ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
        for(uint64_t i = 0; i < list.size(); i++){
            arr[i] = list[i];
        }
        arr[list.size()] = nullptr; // null terminate the arraya

        return arr;

    }

    /*
        now there is some varName also after block of union definition
    */


    UnionDefinitionNode* unionDef;

    // generate union definition ast first
    if(tagNameExist){ // actual tagName union definition ast
        unionDef = new UnionDefinitionNode(tagNameExist , tagName.data , unionBlock);
    } else{ // anonTagName union definition ast
        unionDef = new UnionDefinitionNode(tagNameExist , *anonTagName , unionBlock);
    }

    // push union definition node to "list"
    list.push_back(unionDef);

    /*
        if it came from dataTypeDecl, it has some specifiers like extern/volatile/restrict/const/static , we here need to validate it and the manually add union tagName to it in the dataType object
    */

    // validity of data type prop array is STILL not checked so far

    // helperDeclName validaiton algo
    if(helperDeclName){ // validate if exist
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
        helperDeclName->trKeywordArray.push_back(KEYWORD_UNION);
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }

    getVarAgain:

    /*
        so, now var decl also exist with union definition
        if data type prop object was alr present, it is alr validated in code just before this comment, so now if it exists,we r good to go, if it doesnt exists, we need to create it here manually for the var decl
    */


    // union var decl
    varNameHolder* unionVarName = new varNameHolder(*this);

    // if data type prop doesnt exists, create it
    if(!helperDeclName){

        // new data type prop object
        helperDeclName = new dataTypeHolder(*this);

        // manually add keyword "union" and it's corresponding base name to the newly created dataType prop object
        helperDeclName->trKeywordArray.push_back(KEYWORD_UNION);
        helperDeclName->trBaseArray.push_back(tagNameExist ? tagName.data : *anonTagName);
    }

    // push var decl ast to "list"
    list.push_back(unionVarName->getVarName(*helperDeclName, false));

    // if , is present, it means, there is multi var decl
    if(tokens[currentPos].type == COMMA){

        currentPos++; // skipp ,
        goto getVarAgain; // make another ast for multi var decl
    }

    // ; expected
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; here to close it\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return union definition node for startParsing to collect

    ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
    for(uint64_t i = 0; i < list.size(); i++){
        arr[i] = list[i];
    }
    arr[list.size()] = nullptr; // null terminate the arraya

    return arr;
}

// parsing typedef
ASTNode** Parser::parseTypedef() {

    string helper;
    

    // set tracker to true when entering typedef parsing
    typedDefTracker = true;

    currentPos++; // skip typedef keyword

    vector<ASTNode*> list; // list to store struct decl node and maybe var decl nodes also if exist

    // index to store index of token just after "typedef"
    int tempIndexHolder = currentPos;

    // generate data type prop object 
    dataTypeHolder* original = new dataTypeHolder(*this);

    // parse data type properly
    int retValueDecl = original->getDataType();

    // validate this parsed data type prop object
    original->isCurrentTypeValid();

    // it is struct/union/enum definition
    if(retValueDecl == 2){ 
        
        // convert tempASTStorage to arrya and push all to list
        for(uint64_t i=0 ; i<tempASTStorage.size(); i++){            

            /*
                so, now, it is typedef with struct/union/enum ast
                the corresponding struct/union/enum ast is stored in tempASTStorage array
                the first ast in that array is the base struct/union/enum definition ast, and the further ones, if present, might be the varDecl ast, but since it is part of typedef, we will convert those varDecl ast to typedef alias properly 
            */
           
            string tagNameStore;
            
            if(i == 0){ // first one is struct/union/enum definition itself
                // tagNameStore = dynamic_cast<StructDefinitionNode*>(tempASTStorage[i])->tagName;
                list.push_back(tempASTStorage[i]); // add the struct to the list as it is the foundation for typedef                
            } else{ // now, convert properly

                // convert ast stored in tempASTStorage to variable declnode
                VariableDeclarationNode* parentObj = dynamic_cast<VariableDeclarationNode*>(tempASTStorage[i]);

                varNameHolder* alias = &parentObj->varName; // store the varName object of the (so called) var decl, coz need to acces this to generate typedef ast 

                // typedef hashmap object
                tdMapPair* rhs = new tdMapPair;
                rhs->nameProp = alias; // store the varName string in the tdMapPair

                // helper to store index properly
                int helperStorageInt = tempIndexHolder; 

                // below code is copy pasted to specially deal with struct/union/enum
                { // collecting the data type and it's prop in this block

                    addAgain2: // need to continue evaluating data type props when tdMap key is found as data type prop

                    // adding data type or any of it's property to the declProp array
                    while(isThisTokenDataTypeOrPropToken(tokens[tempIndexHolder])){
                        rhs->declProp.push_back(tokens[tempIndexHolder].data);

                        tempIndexHolder++; // advancing 1 token
                    }

                    // check if token is struct/union/enum
                    if(tokens[tempIndexHolder].type == KEYWORD_STRUCT || tokens[tempIndexHolder].type == KEYWORD_UNION || tokens[tempIndexHolder].type == KEYWORD_ENUM){

                        // push struct/union/enum to the declProp array
                        rhs->declProp.push_back(tokens[tempIndexHolder].data);

                        tempIndexHolder++; // skip struct/union/enum keyword

                        if(tokens[tempIndexHolder].type == ID){ // struct/union/enum tagName also exist
                            rhs->declProp.push_back(tokens[tempIndexHolder].data); // push the tagName to the declProp array prop

                            tempIndexHolder++; // skip tagName
                        } else{
                            // it is anon struct/union/enum def with typedef, need to give it a name myself
                            // get the anon struct/union/enum name from the latest definition using dynamic cast

                            if(StructDefinitionNode* structDef = dynamic_cast<StructDefinitionNode*>(list[0])) { // for struct definition ast
                                rhs->declProp.push_back(structDef->tagName);
                            } else if(UnionDefinitionNode* unionDef = dynamic_cast<UnionDefinitionNode*>(list[0])) { // for union definition ast
                                rhs->declProp.push_back(unionDef->tagName);
                            } else if(EnumDefinitionNode* enumDef = dynamic_cast<EnumDefinitionNode*>(list[0])) { // for enum definition ast
                                rhs->declProp.push_back(enumDef->tagName);
                            }
                        }

                    }

                    if(tokens[tempIndexHolder].type == ID){ // ID found after data type or it's prop, has to be either alias or a key in tdMap
                        if(tokens[tempIndexHolder+1].type == SEMICOLON){ // if ; present after ID, it has to be alias
                            // it is the situation of normal typedef usecase (no complex expr in alias)

                            // generate typedef ast
                        } else if(isThisStringPresentAsKeyInTdMap(tokens[tempIndexHolder].data)){ // ID found as key in tdMap
                            rhs->declProp.push_back(tokens[tempIndexHolder].data); // push ID to declProp array

                            tempIndexHolder++; // skip ID

                            goto addAgain2; // keep evaluating data type props again
                        }
                    }


                } 
                
                
                
                // Add the current declarator's typedef to the list and map
                list.push_back(new TypedefDeclarationNode(rhs->declProp , rhs->nameProp));

                if(i != tempASTStorage.size() - 1){ // update tempIndexHolder if more present
                    tempIndexHolder = helperStorageInt; 
                    continue;
                }
                        
                ASTNode** arr = new ASTNode*[list.size() + 1]; // +1 for nullptr termination
                for(uint64_t i = 0; i < list.size(); i++){
                    arr[i] = list[i];
                }
                arr[list.size()] = nullptr; // null terminate the arraya            
            
                // add it properly to the tdHashMap
                string typedefAliasName = rhs->nameProp->namePropArray[0].varName;
                tdMap[typedefAliasName].push_back(*rhs); 
            
                // clear the global ast array
                tempASTStorage.clear();            

                if(tokens[currentPos].type == SEMICOLON) currentPos++; // skip ;
                
                // typedef completed, update typedef tracker
                typedDefTracker = false; 

                // return
                return arr;

            }

            

        }                

    }     

    bool isFirstDeclator = true; // Track if this is the first declarator in multi-declarator typedef

    short indexSaver = tempIndexHolder; // Save index before looping through data type tokens

    multiTypeDefDeclLabel: // Label for handling multiple declarators

    // create a new fresh varNameHolder object
    varNameHolder* alias = new varNameHolder(*this);

    // parse the name of the alias (NOTE: if it is first typedef alias, no need to reset static variables, need for the 2nd and beyond in case of multiple ones)
    DeclarationNode* parentObj = alias->getVarName(*original, false, !isFirstDeclator); 
    // while paring name, we will know if it is either variable or func

    // creating a new object to be on the pair side of the key-pair hashmap of typedef
    tdMapPair* rhs = new tdMapPair;

    // using dynamic cast to access the nameProp object inside varNameObject properly
    if(VariableDeclarationNode* varObj = dynamic_cast<VariableDeclarationNode*>(parentObj)){ // if it is var decl, then use variable dynamic cast
        rhs->nameProp = &varObj->varName;
    } else if(FunctionDeclarationNode* funcObj = dynamic_cast<FunctionDeclarationNode*>(parentObj)){ // if it is func decl, then use function dynamic cast
        rhs->nameProp = &funcObj->funcName;
    }


    /*
        at this point, we have alr filled the name part of the "rhs", now the vector<string> part is reamining of the same
        to do this, we will use our data type parsing algo here to parse it properly, since it has to be in sequential form, it will work

        NOTE: declProp is the array in which data type or it's prop need to be pushed, hence that is used in code block under this 
    */

    { // collecting the data type and it's prop in this block

        addAgain: // need to continue evaluatinng data type props when tdMap key is found as data type prop
        
        // adding data type or any of it's property to thge declProp array
        while(isThisTokenDataTypeOrPropToken(tokens[tempIndexHolder])){
            rhs->declProp.push_back(tokens[tempIndexHolder].data);

            tempIndexHolder++; // advancing 1 token
        }        

        // check if token is sturct/union/enum
        if(tokens[tempIndexHolder].type == KEYWORD_STRUCT || tokens[tempIndexHolder].type == KEYWORD_UNION || tokens[tempIndexHolder].type == KEYWORD_ENUM){

            // this is pending for now

            // push struct/union/enum to the declProp array
            rhs->declProp.push_back(tokens[tempIndexHolder].data);

            tempIndexHolder++; // skip struct/union/enum keyword

            if(tokens[tempIndexHolder].type == ID){ // struct/union/enum tagName also exist
                rhs->declProp.push_back(tokens[tempIndexHolder].data); // push the tagName to the declProp array prop

                tempIndexHolder++; // skip tagName
            } else{
                // it is anon struct/union/enum def with typedef, need to give it a name myself
                // get the anon struct/union/enum name from the latest definition using dynamic cast

                if(StructDefinitionNode* structDef = dynamic_cast<StructDefinitionNode*>(list.back())) { // for struct definition ast
                    rhs->declProp.push_back(structDef->tagName);
                } else if(UnionDefinitionNode* unionDef = dynamic_cast<UnionDefinitionNode*>(list.back())) { // for union definition ast
                    rhs->declProp.push_back(unionDef->tagName);
                } else if(EnumDefinitionNode* enumDef = dynamic_cast<EnumDefinitionNode*>(list.back())) { // for enum definition ast
                    rhs->declProp.push_back(enumDef->tagName);
                }
            }

        }

        if(tokens[tempIndexHolder].type == ID){ // ID found after data type or it's prop, has to be either alias or a key in tdMap
            if(tokens[tempIndexHolder+1].type == SEMICOLON){ // if ; present after ID, it has to be alias
                // it is the situation of normal typedef usecase (no complex expr in alias)

                // generate typedef ast
            } 
            
            else if(!isFirstDeclator && tokens[tempIndexHolder].data == helper){
                // now it reached name of the prev alias , skip and do NOT push to decl prop array
            }

            // repalcing if with else if
            else if(isThisStringPresentAsKeyInTdMap(tokens[tempIndexHolder].data)){ // ID found as key in tdMap                
                rhs->declProp.push_back(tokens[tempIndexHolder].data); // push ID to declProp array

                tempIndexHolder++; // skip ID

                goto addAgain; // keep evaluating data type props again
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

        if(isFirstDeclator){
            helper = typedefAliasName;
        }

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

        // typedef completed, update typedef tracker
        typedDefTracker = false; 

        // return
        return arr;
    } else{
        cout << "Expected ; or , after typedef declarator\n";
        exit(1);
    }    

    // typedef completed, update typedef tracker
    typedDefTracker = false; 

    // return, never gonna reach here anyways
    return nullptr;
}

// Jump statements
//parsing return
StatementNode* Parser::parseReturn() {
    currentPos++; // skip keyword return

    // decl exprNode
    ExpressionNode* retExpr = nullptr;

    // it has something after "return", parse it properly
    if(tokens[currentPos].type != SEMICOLON){
        retExpr = parseExpression(0, false, -1);
    }

    // ; expected after expression
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after return statement\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return
    return new ReturnStatementNode(retExpr);
}

// parsing continue 
StatementNode* Parser::parseContinue() {
    currentPos++; // skip keyword continue

    // ; expected after "continue"
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after continue\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return
    return new ContinueStatementNode();
}

// parsing break
StatementNode* Parser::parseBreak() {
    currentPos++; // skip keyword break

    // ; expected after "break"
    if(tokens[currentPos].type != SEMICOLON){
        cout << "Expected ; after break\n";
        exit(1);
    }

    currentPos++; // skip ;

    // return
    return new BreakStatementNode();
}

// parsing goto
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
    
    // Expect ;
    if(currentPos >= tokens.size() || tokens[currentPos].type != SEMICOLON) {
        cout << "Expected ';' after goto statement\n";
        exit(1);
    }
    currentPos++;
    
    // Create and return goto statement node
    return new GotoStatementNode(labelName);
}
    
// parsing label
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



