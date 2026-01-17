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

ParameterNode::ParameterNode(dataTypeHolder* type, varNameHolder* name)
    : paramType(*type), paramName(*name) {
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
    paramType.isCurrentTypeValid();

    if(parser.tokens[parser.currentPos].type != COMMA && parser.tokens[parser.currentPos].type != RPAREN){ // evaluate ONYL if names are available after data type
        // get name
        paramName.getVarName(paramType , true);
    }
    

    if(parser.tokens[parser.currentPos].type == COMMA){ //  multiple decl 
        ParameterNode temp(&paramType , &paramName);
        myParamArray.push_back(temp);
        
        if(parser.tokens[parser.currentPos+1].type == OP_DOT){ // varaidic func
            if(parser.tokens[parser.currentPos+2].type != OP_DOT || parser.tokens[parser.currentPos+3].type != OP_DOT){ // must be ... to be variadic
                // error
                cout << "Must be ...\n";
                exit(1);
            }
            parser.currentPos += 4; // skip , and ...
            if(parser.tokens[parser.currentPos].type != RPAREN){
                // error
                cout << "Expected )\n";
                exit(1);
            }
            // currently at )
            
        } else {
            parser.currentPos++; // skip ,
            goto multiParams;
        }
        
    } else if(parser.tokens[parser.currentPos].type == RPAREN){
        ParameterNode temp(&paramType , &paramName);
        myParamArray.push_back(temp);
        // currently at )
    } else{
        cout << "Expected either , or ) after param name\n";
        exit(1);
    }

    return myParamArray;

    
}