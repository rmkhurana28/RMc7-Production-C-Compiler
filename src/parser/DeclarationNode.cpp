#include "DeclarationNode.h"
#include "Parser.h"
#include "Helper.h"
#include <iostream>
using namespace std;

// DeclarationNode implementations

VariableDeclarationNode::VariableDeclarationNode(dataTypeHolder* type , varNameHolder* name , bool isInitBool , ExpressionNode* expr , bool isArr , short unsigned arrDimensions)
    : varDeclType(*type), varName(*name), isInit(isInitBool), initExpr(expr), isArray(isArr), arrDim(arrDimensions) {
}

FunctionDeclarationNode::FunctionDeclarationNode(dataTypeHolder* retType, varNameHolder* name, vector<ParameterNode> params, bool isVar)
    : funcDeclType(*retType), funcName(*name), paramsArray(params), isVariadic(isVar) {
}

FunctionDefinitionNode::FunctionDefinitionNode(dataTypeHolder* retType, varNameHolder* name, vector<ParameterNode> params, bool isVar, BlockExpressionNode* body)
    : funcDefType(*retType), funcName(*name), paramsArray(params), isVariadic(isVar), funcBody(body) {
}

FunctionDefinitionNode::~FunctionDefinitionNode() {
    delete funcBody;
}

ForwardDeclarationNode::ForwardDeclarationNode(TokenType type, string tag)
    : declType(type), tagName(tag) {
}

StructDefinitionNode::StructDefinitionNode(bool hasTag, string tag, BlockExpressionNode* blk)
    : isTagNamePresent(hasTag), tagName(tag), block(blk) {
}

StructDefinitionNode::~StructDefinitionNode() {
    delete block;
}

UnionDefinitionNode::UnionDefinitionNode(bool hasTag, string tag, BlockExpressionNode* blk)
    : isTagNamePresent(hasTag), tagName(tag), block(blk) {
}

UnionDefinitionNode::~UnionDefinitionNode() {
    delete block;
}

EnumDefinitionNode::EnumDefinitionNode(bool hasTag, string tag, EnumBlockExpressionNode* blk)
    : isTagNamePresent(hasTag), tagName(tag), block(blk) {
}

EnumDefinitionNode::~EnumDefinitionNode() {
    delete block;
}

TypedefDeclarationNode::TypedefDeclarationNode(vector<string> tokens, varNameHolder* name)
    : baseTokens(tokens), nameProp(*name) {
}

ParameterNode::ParameterNode(dataTypeHolder* type, varNameHolder* name)
    : paramType(*type), paramName(*name) {
}

ParameterNode& ParameterNode::operator=(const ParameterNode& other) {
    if (this != &other) {
        paramType = other.paramType;
        paramName = other.paramName;
    }
    return *this;
}


vector<ParameterNode> ParameterNode::evaluateParams(Parser& parser){
    vector<ParameterNode> myParamArray;

    if(parser.tokens[parser.currentPos].type == RPAREN){ // unknows params ()
        return myParamArray;
    }

    multiParams:

    // create fresh objects for each parameter
    dataTypeHolder paramType(parser);
    varNameHolder paramName(parser);

    // get data type
    paramType.getDataType();

    // validate data type
    short check = paramType.isCurrentTypeValid();
    if(check == -1){
        cout << "Param data type invalid\n";
        exit(1);
    }
    if(check == 2){

        
        // special check if the param is just void, required since void(just "void") generally not allowed for variable data type
        if(paramType.baseTypeArray.size() == 1 && paramType.baseTypeArray[0] == KEYWORD_VOID){
            // void is valid as param, but without varname

            if(parser.tokens[parser.currentPos].type == ID){
                cout << "Param name not valid for void type\n";
                exit(1);
            }
        } else {
            cout << "Param data type not valid for variable\n";
            exit(1);
        }
    }

    
    
    if(parser.tokens[parser.currentPos].type != COMMA && parser.tokens[parser.currentPos].type != RPAREN){ // evaluate ONYL if names are available after data type

        // get name
        paramName.getVarName(paramType , true);

        // validate name prop
        short check = paramName.checkValidity();
        if(check == -1){
            cout << "Param name invalid\n";
            exit(1);
        }
        
        if(check == 2){
            cout << "Param name not valid for variable\n";
            exit(1);
        }
    }
    
    
    if(parser.tokens[parser.currentPos].type == COMMA){ //  multiple decl 
        ParameterNode temp(&paramType , &paramName);
        myParamArray.push_back(temp);
        
        // check if this is varaidic func
        if(parser.tokens[parser.currentPos+1].type == OP_DOT){ // varaidic must start with .
            if(parser.tokens[parser.currentPos+2].type != OP_DOT || parser.tokens[parser.currentPos+3].type != OP_DOT){ // must be ... to be variadic

                // full varaidic not found, but dot found
                cout << "Must be ...\n";
                exit(1);
            }

            parser.currentPos += 4; // skip , and ...

            // after varaidic param, it should close the paren
            if(parser.tokens[parser.currentPos].type != RPAREN){
                
                // expected ) to close the func param bracket
                cout << "Expected )\n";
                exit(1);
            }
            
            // currently at )
            
        } else { // it is multi param, skip , and proceed toe vlauate further params
            parser.currentPos++; // skip ,
            goto multiParams;
        }
        
    } else if(parser.tokens[parser.currentPos].type == RPAREN){ // add param to the parameter node
        ParameterNode temp(&paramType , &paramName);
        myParamArray.push_back(temp);
        // currently at )
    } else{
        cout << "Expected either , or ) after param name\n";
        exit(1);
    }

    return myParamArray;

    
}