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
        this->parseCurrentDecl(); // parse declaration(s)
        
        // Add all nodes from allAST to allDeclNodes
        for(auto node : this->allAST) {
            allDeclNodes.push_back(static_cast<DeclarationNode*>(node));
        }
        this->allAST.clear(); // clear for next declaration
    }

    // generating program node containing an array of all decl nodes
    ProgramNode* myRootNode = new ProgramNode(allDeclNodes);

    // returning the program node as root node
    return myRootNode;
}

DeclarationNode* Parser::parseCurrentDecl(){
    // use if else to find the best parser for current node
    if(isThisTokenDataTypeOrPropToken(this->tokens[this->currentPos])){ // found some data type or prop
        // call the parser fucntion with data type as first token
        this->parseDataTypeFoundDeclaration();
        return nullptr; // allAST now contains all nodes
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

