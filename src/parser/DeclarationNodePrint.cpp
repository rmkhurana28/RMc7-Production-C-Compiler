#include "DeclarationNode.h"
#include "StatementNode.h"
#include "../lexer/Token.h"
#include <sstream>

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
    
    // Print storage class (NO stars - they're in Var Name modifiers)
    for(auto sc : varDeclType.storageClassArray) {
        out << tokenToReadable(sc) << " ";
    }
    
    // Print sign modifiers (NO stars - they're in Var Name modifiers)
    for(auto sm : varDeclType.signModifiersArray) {
        out << tokenToReadable(sm) << " ";
    }
    
    // Print size modifiers (NO stars - they're in Var Name modifiers)
    for(auto szm : varDeclType.sizeModifiersArray) {
        out << tokenToReadable(szm) << " ";
    }
    
    // Print base type (NO stars here as per requirement)
    if(!varDeclType.baseTypeArray.empty()) {
        out << tokenToReadable(varDeclType.baseTypeArray[0]) << " ";
    } else if(!varDeclType.trBaseArray.empty()) {
        out << varDeclType.trBaseArray[0] << " ";
    } else if(!varDeclType.tdNew.empty()) {
        out << varDeclType.tdNew[0] << " ";
    }
    
    // Print type qualifiers WITH their stars (only for first var in multi-decl)
    for(auto tq : varDeclType.typeQualifiersArray) {
        out << tokenToReadable(tq);
        // Print stars associated with this qualifier
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
    
    // Print array size expressions
    if(isArray) {
        out << "      Array Sizes:\n";
        int arrayCount = 0;
        for(const auto& prop : varName.namePropArray) {
            if(prop.type == ARRAY) {
                arrayCount++;
                out << "        [Dimension " << arrayCount << "]: ";
                if(prop.arrayExpr) {
                    out << "\n";
                    ExpressionNode* exprNode = dynamic_cast<ExpressionNode*>(prop.arrayExpr);
                    if(exprNode) {
                        exprNode->print(out, "          ");
                    } else {
                        out << "          ERROR: Not an ExpressionNode\n";
                    }
                } else {
                    out << "NULL\n";
                }
            }
        }
    }
    
    // Print initialization status
    out << "      IsInit: " << (isInit ? "true" : "false") << "\n";
    
    // Print AST (initialization expression)
    out << "      AST: ";
    if (initExpr) {
        out << "\n";
        initExpr->print(out, "        ");
        out << "\n";
    } else {
        out << "NULL\n";
    }
    
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
            
            // Print sign and size modifiers
            for(const auto& sm : param.paramType.signModifiersArray) {
                out << tokenToReadable(sm) << " ";
            }
            for(const auto& szm : param.paramType.sizeModifiersArray) {
                out << tokenToReadable(szm) << " ";
            }
            
            out << tokenToReadable(param.paramType.baseTypeArray[0]);
            
            // For anonymous parameters, print stars from starDataArray
            bool isAnonymous = param.paramName.namePropArray.empty() || 
                              (param.paramName.namePropArray.size() == 1 && param.paramName.namePropArray[0].type != VAR_NAME);
            if(isAnonymous) {
                // Find stars - check base type first, then size/sign modifiers (for multi-word types like unsigned long)
                int starsFound = 0;
                TokenType baseType = param.paramType.baseTypeArray[0];
                
                // First check if base type has stars directly
                for(const auto& starData : param.paramType.starDataArray) {
                    if(starData.typeBeforeStar == baseType && starData.numOfStars > 0) {
                        starsFound = starData.numOfStars;
                        break;
                    }
                }
                
                // If no stars found with base type, check size/sign modifiers (for cases like "unsigned long ****")
                if(starsFound == 0) {
                    for(const auto& starData : param.paramType.starDataArray) {
                        TokenType t = starData.typeBeforeStar;
                        if((t == KEYWORD_SHORT || t == KEYWORD_LONG || t == KEYWORD_SIGNED || t == KEYWORD_UNSIGNED) 
                           && starData.numOfStars > 0) {
                            starsFound = starData.numOfStars;
                            break;
                        }
                    }
                }
                
                if(starsFound > 0) {
                    out << " ";
                    for(int j = 0; j < starsFound; j++) {
                        out << "*";
                    }
                }
            }
        } else if(!param.paramType.trBaseArray.empty()) {
            out << param.paramType.trBaseArray[0];
        }
        
        // Find and print parameter name first, then modifiers
        string paramName = "";
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == VAR_NAME) {
                paramName = nameProp.varName;
            }
        }
        
        // Print name if present
        if(!paramName.empty()) {
            out << " " << paramName;
        }
        
        // Print modifiers (pointers, arrays, functions) after name
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == POINTOR) {
                for(int j = 0; j < nameProp.numPointor; j++) {
                    out << "*";
                }
            } else if(nameProp.type == ARRAY) {
                out << "[]";
            } else if(nameProp.type == FUNC) {
                out << "()";
            }
        }
        
        // For named params with simple pointers, check if stars are in starDataArray but not in namePropArray
        // This happens for declarations like "int ***_ppp" where stars are stored with base type
        int starsFromStarData = 0;
        bool hasPointorInNameProp = false;
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == POINTOR) {
                hasPointorInNameProp = true;
                break;
            }
        }
        if(!hasPointorInNameProp && !paramName.empty()) {
            // Check starDataArray for stars
            TokenType baseType = param.paramType.baseTypeArray.empty() ? HELPER_TOKEN : param.paramType.baseTypeArray[0];
            for(const auto& starData : param.paramType.starDataArray) {
                if(starData.typeBeforeStar == baseType && starData.numOfStars > 0) {
                    starsFromStarData = starData.numOfStars;
                    break;
                }
            }
            // If no stars with base type, check size/sign modifiers
            if(starsFromStarData == 0) {
                for(const auto& starData : param.paramType.starDataArray) {
                    TokenType t = starData.typeBeforeStar;
                    if((t == KEYWORD_SHORT || t == KEYWORD_LONG || t == KEYWORD_SIGNED || t == KEYWORD_UNSIGNED) 
                       && starData.numOfStars > 0) {
                        starsFromStarData = starData.numOfStars;
                        break;
                    }
                }
            }
            // Print the stars if found
            if(starsFromStarData > 0) {
                for(int j = 0; j < starsFromStarData; j++) {
                    out << "*";
                }
            }
        }
        
        // Show declarator sequence in brackets
        if(!param.paramName.namePropArray.empty() || starsFromStarData > 0) {
            out << " [";
            bool first = true;
            for(size_t j = 0; j < param.paramName.namePropArray.size(); j++) {
                if(!first) out << ", ";
                first = false;
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
            // Add POINTOR from starDataArray if not already in namePropArray
            if(starsFromStarData > 0) {
                if(!first) out << ", ";
                out << "POINTOR(" << starsFromStarData << ")";
            }
            out << "]";
        }
        out << "\n";
        
        // Recursively print nested function parameters
        int nestedFuncIndex = 0;
        for(const auto& nameProp : param.paramName.namePropArray) {
            if(nameProp.type == FUNC && !nameProp.funcParams.empty()) {
                nestedFuncIndex++;
                out << indent << "  Nested Function Parameters (" << nameProp.funcParams.size() << ") [FUNC #" << nestedFuncIndex << "]:\n";
                printParametersRecursive(out, nameProp.funcParams, indent + "    ");
            }
        }
    }
}

// Modified VariableDeclarationNode print to call the helper
void VariableDeclarationNode::printParameters(ofstream& out) {
    int funcIndex = 0;
    for(const auto& prop : varName.namePropArray) {
        if(prop.type == FUNC && !prop.funcParams.empty()) {
            funcIndex++;
            out << "      Parameters (" << prop.funcParams.size() << ") [FUNC #" << funcIndex << "]:\n";
            printParametersRecursive(out, prop.funcParams, "        ");
        }
    }
}

void FunctionDeclarationNode::print(ofstream& out) {
    out << "      Return Type: ";

    // Print return type
    if(!funcDeclType.baseTypeArray.empty()) {
        for(auto bt : funcDeclType.baseTypeArray) {
            out << tokenToReadable(bt) << " ";
        }
    } else if(!funcDeclType.trKeywordArray.empty() && !funcDeclType.trBaseArray.empty()) {
        // For struct/union return types
        out << tokenToReadable(funcDeclType.trKeywordArray[0]) << " " << funcDeclType.trBaseArray[0] << " ";
    }
    
    // Print function name with full declarator sequence
    out << "\n      Function Name: ";
    for(const auto& prop : funcName.namePropArray) {
        if(prop.type == VAR_NAME) {
            out << prop.varName;
        }
    }
    
    // Print declarator sequence in brackets
    out << " [";
    for(size_t i = 0; i < funcName.namePropArray.size(); i++) {
        if(i > 0) out << ", ";
        const auto& prop = funcName.namePropArray[i];
        switch(prop.type) {
            case VAR_NAME: 
                out << "VAR_NAME"; 
                break;
            case POINTOR: 
                out << "POINTOR(" << prop.numPointor << ")"; 
                break;
            case ARRAY: 
                out << "ARRAY";
                if(prop.arrayExpr) {
                    out << "(index)";
                } else {
                    out << "(N/A)";
                }
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
    out << "]";
    
    // Print main function parameters - find the first FUNC in the declarator sequence
    out << "\n      Parameters: ";
    
    // Find the first FUNC modifier which contains the actual parameters
    vector<ParameterNode> actualParams;
    bool foundParams = false;
    for(const auto& prop : funcName.namePropArray) {
        if(prop.type == FUNC) {
            actualParams = prop.funcParams;
            foundParams = true;
            break;
        }
    }
    
    // If no FUNC found in modifiers, fall back to paramsArray
    if(!foundParams && !paramsArray.empty()) {
        actualParams = paramsArray;
        foundParams = true;
    }
    
    if(!foundParams || actualParams.empty()) {
        out << "()";
    } else {
        out << "(" << actualParams.size() << ")";
        if(isVariadic) {
            out << " [variadic]";
        }
        out << "\n";
        printParametersRecursive(out, actualParams, "        ");
    }
    
    // Print return type modifiers (like pointer-to-function)
    // This shows parameters that are part of the function's return type
    bool hasReturnTypeModifiers = false;
    for(const auto& prop : funcName.namePropArray) {
        if(prop.type == FUNC && !prop.funcParams.empty() && &prop != &funcName.namePropArray[1]) {
            // Skip the first FUNC (that's the main function), print others
            hasReturnTypeModifiers = true;
        }
    }
    
    if(hasReturnTypeModifiers) {
        out << "\n      Return Type Modifiers:\n";
        int funcIndex = 0;
        for(const auto& prop : funcName.namePropArray) {
            if(prop.type == FUNC) {
                funcIndex++;
                if(funcIndex > 1 && !prop.funcParams.empty()) { // Skip first FUNC
                    out << "        Function signature (" << prop.funcParams.size() << " params):\n";
                    printParametersRecursive(out, prop.funcParams, "          ");
                }
            }
        }
    }
    out << "\n";
}

void FunctionDefinitionNode::print(ofstream& out) {
    out << "      Return Type: ";

    // Print storage class
    for(auto sc : funcDefType.storageClassArray) {
        out << tokenToReadable(sc) << " ";
    }

    // Print sign modifiers
    for(auto sm : funcDefType.signModifiersArray) {
        out << tokenToReadable(sm) << " ";
    }

    // Print size modifiers
    for(auto szm : funcDefType.sizeModifiersArray) {
        out << tokenToReadable(szm) << " ";
    }

    // Print qualifiers
    for(auto tq : funcDefType.typeQualifiersArray) {
        out << tokenToReadable(tq) << " ";
    }

    // Print base type
    if(!funcDefType.baseTypeArray.empty()) {
        out << tokenToReadable(funcDefType.baseTypeArray[0]);
    } else if(!funcDefType.trKeywordArray.empty() && !funcDefType.trBaseArray.empty()) {
        // For struct/union return types
        out << tokenToReadable(funcDefType.trKeywordArray[0]) << " " << funcDefType.trBaseArray[0];
    }

    // Print stars from return type
    for(const auto& starData : funcDefType.starDataArray) {
        for(int j = 0; j < starData.numOfStars; j++) {
            out << "*";
        }
    }

    out << "\n";
    
    out << "      Function Name: ";
    for(const auto& prop : funcName.namePropArray) {
        if(prop.type == VAR_NAME) {
            out << prop.varName;
        }
    }
    out << "\n";
    
    out << "      Parameters (" << paramsArray.size() << "):\n";
    if(!paramsArray.empty()) {
        printParametersRecursive(out, paramsArray, "        ");
    } else {
        out << "        (no parameters)\n";
    }
    
    if(isVariadic) {
        out << "      Variadic: Yes\n";
    }
    
    out << "      Function Body (" << (funcBody ? funcBody->expressions.size() : 0) << " AST nodes):\n";
    if(funcBody && !funcBody->expressions.empty()) {
        for(size_t i = 0; i < funcBody->expressions.size(); i++) {
            out << "\n";
            out << "        ---- Body Node #" << (i+1) << " ----\n";
            if(funcBody->expressions[i]) {
                // Check if it's a statement
                StatementNode* stmt = dynamic_cast<StatementNode*>(funcBody->expressions[i]);
                if(stmt) {
                    stmt->print(out, "        ");
                } else {
                    // Check if it's a declaration
                    DeclarationNode* decl = dynamic_cast<DeclarationNode*>(funcBody->expressions[i]);
                    if(decl) {
                        // Write to a temporary file to capture output
                        ofstream tempFile(".temp_decl_output.txt");
                        decl->print(tempFile);
                        tempFile.close();

                        // Read back and add indentation to each line
                        ifstream tempRead(".temp_decl_output.txt");
                        string line;
                        while(getline(tempRead, line)) {
                            if(!line.empty()) {
                                out << "  " << line << "\n";
                            }
                        }
                        tempRead.close();
                        remove(".temp_decl_output.txt");
                    } else {
                        // Check if it's an expression (e.g., BlockExpressionNode for nested blocks)
                        ExpressionNode* expr = dynamic_cast<ExpressionNode*>(funcBody->expressions[i]);
                        if(expr) {
                            expr->print(out, "        ");
                        } else {
                            out << "        [Unknown node type]\n";
                        }
                    }
                }
            } else {
                out << "        [NULL node]\n";
            }
        }
    } else {
        out << "        (empty body)\n";
    }
    
    out << "\n";
}

void ForwardDeclarationNode::print(ofstream& out) {
    out << "      Declaration Type: Forward Declaration\n";
    out << "      Type: " << tokenToReadable(declType) << "\n";
    out << "      Tag Name: " << tagName << "\n";
}

void StructDefinitionNode::print(ofstream& out) {
    out << "      Declaration Type: Struct Definition\n";
    out << "      Tag Name: " << tagName << "\n";

    if(block && !block->expressions.empty()) {
        out << "      Struct Body (" << block->expressions.size() << " member declarations):\n";

        for(size_t i = 0; i < block->expressions.size(); i++) {
            out << "        ---- Member #" << (i+1) << " ----\n";

            if(block->expressions[i]) {
                // Try to cast to DeclarationNode for proper printing
                DeclarationNode* memberDecl = dynamic_cast<DeclarationNode*>(block->expressions[i]);
                if(memberDecl) {
                    // Write to temp file to add indentation
                    ofstream tempFile("temp_member.txt");
                    memberDecl->print(tempFile);
                    tempFile.close();

                    // Read back with indentation
                    ifstream readFile("temp_member.txt");
                    string line;
                    while(getline(readFile, line)) {
                        out << "        " << line << "\n";
                    }
                    readFile.close();
                } else {
                    out << "        [Non-declaration node]\n";
                }
            } else {
                out << "        [NULL member]\n";
            }
        }
    } else {
        out << "      Struct Body: (empty)\n";
    }
}

void UnionDefinitionNode::print(ofstream& out) {
    out << "      Declaration Type: Union Definition\n";
    out << "      Tag Name: " << tagName << "\n";

    if(block && !block->expressions.empty()) {
        out << "      Union Body (" << block->expressions.size() << " member declarations):\n";

        for(size_t i = 0; i < block->expressions.size(); i++) {
            out << "        ---- Member #" << (i+1) << " ----\n";

            if(block->expressions[i]) {
                // Try to cast to DeclarationNode for proper printing
                DeclarationNode* memberDecl = dynamic_cast<DeclarationNode*>(block->expressions[i]);
                if(memberDecl) {
                    // Write to temp file to add indentation
                    ofstream tempFile("temp_member.txt");
                    memberDecl->print(tempFile);
                    tempFile.close();

                    // Read back with indentation
                    ifstream readFile("temp_member.txt");
                    string line;
                    while(getline(readFile, line)) {
                        out << "        " << line << "\n";
                    }
                    readFile.close();
                } else {
                    out << "        [Non-declaration node]\n";
                }
            } else {
                out << "        [NULL member]\n";
            }
        }
    } else {
        out << "      Union Body: (empty)\n";
    }
}

void EnumDefinitionNode::print(ofstream& out) {
    out << "      Declaration Type: Enum Definition\n";
    out << "      Tag Name: " << tagName << "\n";

    if(block && !block->components.empty()) {
        out << "      Enum Body (" << block->components.size() << " enumerators):\n";

        for(size_t i = 0; i < block->components.size(); i++) {
            out << "        ---- Enumerator #" << (i+1) << " ----\n";
            out << "        Name: " << block->components[i].name << "\n";

            if(block->components[i].value != nullptr) {
                out << "        Value: ";
                block->components[i].value->print(out, "");
                out << "\n";
            } else {
                out << "        Value: (auto-assigned)\n";
            }
        }
    } else {
        out << "      Enum Body: (empty)\n";
    }
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
        } else if(dynamic_cast<ForwardDeclarationNode*>(declarations[i])) {
            out << "Forward Declaration";
        } else if(dynamic_cast<StructDefinitionNode*>(declarations[i])) {
            out << "Struct Definition";
        } else if(dynamic_cast<UnionDefinitionNode*>(declarations[i])) {
            out << "Union Definition";
        } else if(dynamic_cast<EnumDefinitionNode*>(declarations[i])) {
            out << "Enum Definition";
        } else if(dynamic_cast<TypedefDeclarationNode*>(declarations[i])) {
            out << "Typedef Declaration";
        } else {
            out << "Unknown Declaration";
        }

        out << "):\n";
        declarations[i]->print(out);
        out << "\n";
    }
    
    out << "----------------------------------------\n";
}

// Function to print expression statements
void printStatementsToFile(ofstream& out, const vector<StatementNode*>& statements) {
    if(statements.empty()) return;
    
    out << "\n========================================\n";
    out << "        EXPRESSION STATEMENTS       \n";
    out << "========================================\n\n";
    out << "Total Statements: " << statements.size() << "\n\n";
    
    for(size_t i = 0; i < statements.size(); i++) {
        out << "statement #" << (i+1) << ":\n";
        if (statements[i]) {
            statements[i]->print(out);
        } else {
            out << "  NULL statement\n";
        }
        if(i < statements.size() - 1) {
            out << "\n"; // Add gap between statements
        }
    }
    out << "\n----------------------------------------\n";
}

void TypedefDeclarationNode::print(ofstream& out) {
    out << "      Base Type: ";

    // Print base type tokens
    for(const auto& tokenStr : baseTokens) {
        out << tokenStr << " ";
    }

    out << "\n      Alias Name: ";
    // Print alias name - check if VAR_NAME exists
    bool foundName = false;
    for(const auto& prop : nameProp.namePropArray) {
        if(prop.type == VAR_NAME) {
            out << prop.varName;
            foundName = true;
            break;
        }
    }
    if(!foundName) {
        out << "(not set)";
    }

    out << "\n      Alias Shape: [";
    if(nameProp.namePropArray.empty()) {
        out << "empty";
    } else {
        for(size_t i = 0; i < nameProp.namePropArray.size(); i++) {
            if(i > 0) out << ", ";
            const auto& prop = nameProp.namePropArray[i];
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
    }
    out << "]\n";
}
