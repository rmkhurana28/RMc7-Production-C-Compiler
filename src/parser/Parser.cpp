#include "Parser.h"
#include <vector>
#include "Helper.h"

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
    DeclarationNode* temp;
    while(this->currentPos < tokens.size()){
        temp = this->parseCurrentDecl(); // call the perfect parser fucntion detector
        if(temp != nullptr){ // if returned decl node is NOT null , add to the array
            allDeclNodes.push_back(temp);
        }
    }

    // generating program node containing an array of all decl nodes
    ProgramNode myRootNode = ProgramNode(allDeclNodes);

    // returning the program node as root node
    return &myRootNode;
}

DeclarationNode* Parser::parseCurrentDecl(){
    // use if else to find the best parser for current node
    if(isThisTokenDataTypeOrPropToken(this->tokens[this->currentPos])){ // found some data type or prop
        // call the parser fucntion with data type as first token
        this->parseDataTypeFoundDeclaration();
        
    }
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

DeclarationNode* Parser::parseDataTypeFoundDeclaration(){    

    // generate an object to store the current data type
    dataTypeHolder currType = dataTypeHolder(*this);

    // evaluate the data type
    currType.getDataType();

    if(!currType.isCurrentTypeValid()){
        // error mechanism
    }
    // now, the type decl is valid

    

    // decide if this is a fucntion decl or varibale decl
    
    // check if the data type and props are valid

    // if function, check if defined and proceed accordingly
    // function part finished here

    // if variable, check if initialized

    // check if multi-declratations

    // if multi-call , use same data type combination we got for other variable declaraitons also    
    

    // generate all the AST and return accordignly
}