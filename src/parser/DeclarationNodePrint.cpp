#include "DeclarationNode.h"
#include "../lexer/Token.h"

// Helper function to convert token type to readable C keyword
string tokenToReadable(TokenType type) {
    switch(type) {
        case KEYWORD_INT: return "int";
        case KEYWORD_CHAR: return "char";
        case KEYWORD_SHORT: return "short";
        case KEYWORD_LONG: return "long";
        case KEYWORD_FLOAT: return "float";
        case KEYWORD_DOUBLE: return "double";
        case KEYWORD_VOID: return "void";
        case KEYWORD_SIGNED: return "signed";
        case KEYWORD_UNSIGNED: return "unsigned";
        case KEYWORD_CONST: return "const";
        case KEYWORD_VOLATILE: return "volatile";
        case KEYWORD_RESTRICT: return "restrict";
        case KEYWORD_STATIC: return "static";
        case KEYWORD_EXTERN: return "extern";
        case KEYWORD_AUTO: return "auto";
        case KEYWORD_REGISTER: return "register";
        case KEYWORD_TYPEDEF: return "typedef";
        default: return tokenTypeToString(type);
    }
}

// Print function implementations for AST nodes

void VariableDeclarationNode::print(ofstream& out) {
    out << "      Data Type: ";
    
    // Print storage class with stars
    for(auto sc : varDeclType.storageClassArray) {
        out << tokenToReadable(sc);
        // Check if this storage class has stars
        for(const auto& starData : varDeclType.starDataArray) {
            if(starData.typeBeforeStar == sc && starData.numOfStars > 0) {
                for(int i = 0; i < starData.numOfStars; i++) {
                    out << "*";
                }
            }
        }
        out << " ";
    }
    
    // Print sign modifiers with stars
    for(auto sm : varDeclType.signModifiersArray) {
        out << tokenToReadable(sm);
        for(const auto& starData : varDeclType.starDataArray) {
            if(starData.typeBeforeStar == sm && starData.numOfStars > 0) {
                for(int i = 0; i < starData.numOfStars; i++) {
                    out << "*";
                }
            }
        }
        out << " ";
    }
    
    // Print size modifiers with stars
    for(auto szm : varDeclType.sizeModifiersArray) {
        out << tokenToReadable(szm);
        for(const auto& starData : varDeclType.starDataArray) {
            if(starData.typeBeforeStar == szm && starData.numOfStars > 0) {
                for(int i = 0; i < starData.numOfStars; i++) {
                    out << "*";
                }
            }
        }
        out << " ";
    }
    
    // Print base type (NO stars here as per requirement)
    if(!varDeclType.baseTypeArray.empty()) {
        out << tokenToReadable(varDeclType.baseTypeArray[0]) << " ";
    } else if(!varDeclType.trBaseArray.empty()) {
        out << varDeclType.trBaseArray[0] << " ";
    } else if(!varDeclType.tdNew.empty()) {
        out << varDeclType.tdNew[0] << " ";
    }
    
    // Print type qualifiers with stars
    for(auto tq : varDeclType.typeQualifiersArray) {
        out << tokenToReadable(tq);
        for(const auto& starData : varDeclType.starDataArray) {
            if(starData.typeBeforeStar == tq && starData.numOfStars > 0) {
                for(int i = 0; i < starData.numOfStars; i++) {
                    out << "*";
                }
            }
        }
        out << " ";
    }
    
    out << "\n";
    
    // Print variable name only (no stars)
    out << "      Var Name: ";
    for(const auto& prop : varName.namePropArray) {
        if(prop.type == VAR_NAME) {
            out << prop.varName;
            break; // Only print the name, nothing else
        }
    }
    
    // Print sequence in brackets
    out << " [";
    for(size_t i = 0; i < varName.namePropArray.size(); i++) {
        if(i > 0) out << ", ";
        const auto& prop = varName.namePropArray[i];
        switch(prop.type) {
            case VAR_NAME: 
                out << "VAR_NAME"; 
                break;
            case POINTOR: 
                out << "POINTOR(" << prop.numPointor << ")"; 
                break;
            case ARRAY: 
                out << "ARRAY(";
                if(prop.arrayExpr) {
                    // TODO: Extract actual value if simple number/identifier
                    // For now, just show "index" for any expression
                    out << "index";
                } else {
                    out << "N/A";
                }
                out << ")";
                break;
            case FUNC: 
                out << "FUNC(";
                if(!prop.funcParams.empty()) {
                    out << prop.funcParams.size();
                    if(prop.isVariadic) out << ", ...";
                } else if(prop.isVariadic) {
                    out << "...";
                } else {
                    out << "0";
                }
                out << ")";
                break;
        }
    }
    out << "]\n";
    
    // Print array information
    out << "      IsArray: " << (isArray ? "true" : "false") << "\n";
    out << "      ArrayDim: " << arrDim << "\n";
    
    // Print initialization status
    out << "      IsInit: " << (isInit ? "true" : "false") << "\n";
    
    // Print AST (initialization expression)
    out << "      AST: " << (initExpr ? "<AST>" : "NULL") << "\n";
    
    // Print function parameters recursively
    printParameters(out);
}

// Helper function to recursively print parameters
void printParametersRecursive(ofstream& out, const vector<ParameterNode>& params, const string& indent) {
    for(size_t i = 0; i < params.size(); i++) {
        out << indent << "[" << (i + 1) << "] ";
        
        const auto& param = params[i];
        
        // Print parameter type with qualifiers
        if(!param.paramType.baseTypeArray.empty()) {
            // Print qualifiers first
            for(const auto& qual : param.paramType.typeQualifiersArray) {
                out << tokenToReadable(qual) << " ";
            }
            out << tokenToReadable(param.paramType.baseTypeArray[0]);
        } else if(!param.paramType.trBaseArray.empty()) {
            out << param.paramType.trBaseArray[0];
        }
        
        // Print parameter name with modifiers
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == VAR_NAME) {
                out << " " << nameProp.varName;
            } else if(nameProp.type == POINTOR) {
                for(int j = 0; j < nameProp.numPointor; j++) {
                    out << "*";
                }
            } else if(nameProp.type == ARRAY) {
                out << "[]";
            } else if(nameProp.type == FUNC) {
                out << "()";
            }
        }
        
        // Show declarator sequence
        out << " [";
        for(size_t j = 0; j < param.paramName.namePropArray.size(); j++) {
            if(j > 0) out << ", ";
            const auto& nameProp = param.paramName.namePropArray[j];
            switch(nameProp.type) {
                case VAR_NAME: 
                    out << "VAR_NAME"; 
                    break;
                case POINTOR: 
                    out << "POINTOR(" << nameProp.numPointor << ")"; 
                    break;
                case ARRAY: 
                    out << "ARRAY";
                    break;
                case FUNC: 
                    out << "FUNC(" << nameProp.funcParams.size() << ")";
                    break;
            }
        }
        out << "]\n";
        
        // Recursively print nested function parameters
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == FUNC && !nameProp.funcParams.empty()) {
                out << indent << "  Parameters (" << nameProp.funcParams.size() << "):\n";
                printParametersRecursive(out, nameProp.funcParams, indent + "    ");
            }
        }
    }
}

// Modified VariableDeclarationNode print to call the helper
void VariableDeclarationNode::printParameters(ofstream& out) {
    for(const auto& prop : varName.namePropArray) {
        if(prop.type == FUNC && !prop.funcParams.empty()) {
            out << "      Parameters (" << prop.funcParams.size() << "):\n";
            printParametersRecursive(out, prop.funcParams, "        ");
        }
    }
}

void FunctionDeclarationNode::print(ofstream& out) {
    out << "      Return Type: ";
    
    // Print return type
    for(auto bt : funcDeclType.baseTypeArray) {
        out << tokenToReadable(bt) << " ";
    }
    
    // Print function name
    out << "\n      Function Name: ";
    for(const auto& prop : funcName.namePropArray) {
        if(prop.type == VAR_NAME) {
            out << prop.varName;
        }
    }
    
    // Print parameters
    out << "\n      Parameters: ";
    if(paramsArray.empty()) {
        out << "()";
    } else {
        out << "(" << paramsArray.size() << ")";
        if(isVariadic) {
            out << " [variadic]";
        }
        out << "\n";
        printParametersRecursive(out, paramsArray, "        ");
    }
    out << "\n";
}

void ProgramNode::printAST(ofstream& out) {
    out << "\n========================================\n";
    out << "            DECLARATIONS AST           \n";
    out << "========================================\n\n";
    out << "Total Declarations: " << declarations.size() << "\n\n";
    out << "----------------------------------------\n\n";
    
    for(size_t i = 0; i < declarations.size(); i++) {
        out << "declaration #" << (i + 1) << " (";
        
        // Determine declaration type
        if(dynamic_cast<VariableDeclarationNode*>(declarations[i])) {
            out << "Variable Declaration";
        } else if(dynamic_cast<FunctionDeclarationNode*>(declarations[i])) {
            out << "Function Declaration";
        } else if(dynamic_cast<FunctionDefinitionNode*>(declarations[i])) {
            out << "Function Definition";
        } else {
            out << "Unknown Declaration";
        }
        
        out << "):\n";
        declarations[i]->print(out);
        out << "\n";
    }
    
    out << "----------------------------------------\n";
}
