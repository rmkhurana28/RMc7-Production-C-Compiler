#include "ExpressionNode.h"
#include "StatementNode.h"
#include "DeclarationNode.h"
#include "Helper.h"
#include "../lexer/Token.h"
#include <fstream>

using namespace std;

// Helper function to convert operator token to string
string operatorToString(TokenType op) {
    switch(op) {
        // Arithmetic operators
        case OP_PLUS: return "+";
        case OP_MINUS: return "-";
        case OP_STAR: return "*";
        case OP_SLASH: return "/";
        case OP_PERCENT: return "%";
        
        // Assignment operators
        case OP_ASSIGN: return "=";
        case OP_PLUS_EQ: return "+=";
        case OP_MINUS_EQ: return "-=";
        case OP_STAR_EQ: return "*=";
        case OP_SLASH_EQ: return "/=";
        case OP_PERCENT_EQ: return "%=";
        case OP_AND_EQ: return "&=";
        case OP_OR_EQ: return "|=";
        case OP_XOR_EQ: return "^=";
        case OP_LSHIFT_EQ: return "<<=";
        case OP_RSHIFT_EQ: return ">>=";
        
        // Relational operators
        case OP_EQ_EQ: return "==";
        case OP_NOT_EQ: return "!=";
        case OP_LESS: return "<";
        case OP_GREATER: return ">";
        case OP_LESS_EQ: return "<=";
        case OP_GREATER_EQ: return ">=";
        
        // Logical operators
        case OP_AND_AND: return "&&";
        case OP_OR_OR: return "||";
        case OP_NOT: return "!";
        
        // Bitwise operators
        case OP_AND: return "&";
        case OP_OR: return "|";
        case OP_XOR: return "^";
        case OP_TILDE: return "~";
        case OP_LSHIFT: return "<<";
        case OP_RSHIFT: return ">>";
        
        // Increment/Decrement
        case OP_PLUS_PLUS: return "++";
        case OP_MINUS_MINUS: return "--";
        
        // Other
        case COMMA: return ",";
        case OP_QUESTION: return "?";
        case OP_COLON: return ":";
        
        default: return "UNKNOWN_OP";
    }
}

// ============================================================================
// Literal Node Print Functions
// ============================================================================

void IntLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "IntLiteral(" << numLiteralValue << ")";
}

void FloatLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "FloatLiteral(" << floatLiteralValue << ")";
}

void DoubleLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "DoubleLiteral(" << doubleLiteralValue << ")";
}

void CharLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "CharLiteral('" << charLiteralValue << "')";
}

void StringLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "StringLiteral(\"" << stringLiteralValue << "\")";
}

void BoolLiteralNode::print(ofstream& out, const string& indent) const {
    out << indent << "BoolLiteral(" << (boolLiteralValue ? "true" : "false") << ")";
}

// ============================================================================
// Identifier Node Print Function
// ============================================================================

void IdentifierNode::print(ofstream& out, const string& indent) const {
    out << indent << "Identifier(" << identifierName << ")";
}

// ============================================================================
// Binary Operation Node Print Function
// ============================================================================

void BinaryOpNode::print(ofstream& out, const string& indent) const {
    out << indent << "BinaryOp(" << operatorToString(binOpType) << ")\n";
    out << indent << "  Left:\n";
    if (left) {
        left->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Right:\n";
    if (right) {
        right->print(out, indent + "    ");
    } else {
        out << indent << "    NULL";
    }
}

// ============================================================================
// Unary Operation Node Print Function
// ============================================================================

void UnaryOpNode::print(ofstream& out, const string& indent) const {
    out << indent << "UnaryOp(" << operatorToString(unOpType) << ", " 
        << (isPrefix ? "prefix" : "postfix") << ")\n";
    out << indent << "  Operand:\n";
    if (operand) {
        operand->print(out, indent + "    ");
    } else {
        out << indent << "    NULL";
    }
}

// ============================================================================
// Ternary Operation Node Print Function
// ============================================================================

void TernaryOpNode::print(ofstream& out, const string& indent) const {
    out << indent << "TernaryOp(?:)\n";
    out << indent << "  Condition:\n";
    if (condition) {
        condition->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  TrueExpr:\n";
    if (trueExpr) {
        trueExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  FalseExpr:\n";
    if (falseExpr) {
        falseExpr->print(out, indent + "    ");
    } else {
        out << indent << "    NULL";
    }
}

// ============================================================================
// Function Call Node Print Function
// ============================================================================

void FunctionCallNode::print(ofstream& out, const string& indent) const {
    out << indent << "FunctionCall\n";
    out << indent << "  Function:\n";
    if (functionExpr) {
        functionExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Arguments: (" << arguments.size() << ")\n";
    for (size_t i = 0; i < arguments.size(); i++) {
        out << indent << "    [" << (i + 1) << "]:\n";
        if (arguments[i]) {
            arguments[i]->print(out, indent + "      ");
            if (i < arguments.size() - 1) out << "\n";
        } else {
            out << indent << "      NULL";
            if (i < arguments.size() - 1) out << "\n";
        }
    }
}

// ============================================================================
// Array Access Node Print Function
// ============================================================================

void ArrayAccessNode::print(ofstream& out, const string& indent) const {
    out << indent << "ArrayAccess\n";
    out << indent << "  Array:\n";
    if (arrayExpr) {
        arrayExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Index:\n";
    if (indexExpr) {
        indexExpr->print(out, indent + "    ");
    } else {
        out << indent << "    NULL";
    }
}

// ============================================================================
// Member Access Node Print Function
// ============================================================================

void MemberAccessNode::print(ofstream& out, const string& indent) const {
    out << indent << "MemberAccess(.)\n";
    out << indent << "  Object:\n";
    if (objectExpr) {
        objectExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Member: " << memberName;
}

// ============================================================================
// Pointer Member Access Node Print Function
// ============================================================================

void PointerMemberAccessNode::print(ofstream& out, const string& indent) const {
    out << indent << "PointerMemberAccess(->)\n";
    out << indent << "  Pointer:\n";
    if (pointerExpr) {
        pointerExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Member: " << memberName;
}

// ============================================================================
// Cast Node Print Function
// ============================================================================

void CastNode::print(ofstream& out, const string& indent) const {
    out << indent << "Cast(";
    if (typeHolder) {
        // Print type qualifiers with their stars
        for (const auto& tq : typeHolder->typeQualifiersArray) {
            if (tq == KEYWORD_CONST) out << "const";
            else if (tq == KEYWORD_VOLATILE) out << "volatile";
            else if (tq == KEYWORD_RESTRICT) out << "restrict";
            
            // Print stars that come after this qualifier
            for (const auto& starData : typeHolder->starDataArray) {
                if (starData.typeBeforeStar == tq && starData.numOfStars > 0) {
                    for (int i = 0; i < starData.numOfStars; i++) {
                        out << "*";
                    }
                }
            }
            out << " ";
        }
        
        // Print sign modifiers with their stars
        for (const auto& sm : typeHolder->signModifiersArray) {
            if (sm == KEYWORD_SIGNED) out << "signed";
            else if (sm == KEYWORD_UNSIGNED) out << "unsigned";
            
            // Print stars that come after this sign modifier
            for (const auto& starData : typeHolder->starDataArray) {
                if (starData.typeBeforeStar == sm && starData.numOfStars > 0) {
                    for (int i = 0; i < starData.numOfStars; i++) {
                        out << "*";
                    }
                }
            }
            out << " ";
        }
        
        // Print size modifiers with their stars
        for (const auto& szm : typeHolder->sizeModifiersArray) {
            if (szm == KEYWORD_SHORT) out << "short";
            else if (szm == KEYWORD_LONG) out << "long";
            
            // Print stars that come after this size modifier
            for (const auto& starData : typeHolder->starDataArray) {
                if (starData.typeBeforeStar == szm && starData.numOfStars > 0) {
                    for (int i = 0; i < starData.numOfStars; i++) {
                        out << "*";
                    }
                }
            }
            out << " ";
        }
        
        // Print base type with stars
        if (!typeHolder->baseTypeArray.empty()) {
            TokenType bt = typeHolder->baseTypeArray[0];
            if (bt == KEYWORD_INT) out << "int";
            else if (bt == KEYWORD_CHAR) out << "char";
            else if (bt == KEYWORD_VOID) out << "void";
            else if (bt == KEYWORD_FLOAT) out << "float";
            else if (bt == KEYWORD_DOUBLE) out << "double";
            
            // Print stars that come after this base type
            for (const auto& starData : typeHolder->starDataArray) {
                if (starData.typeBeforeStar == bt && starData.numOfStars > 0) {
                    for (int i = 0; i < starData.numOfStars; i++) {
                        out << "*";
                    }
                }
            }
            out << " ";
        }
        
        // Print struct/union/enum if present
        if (!typeHolder->trKeywordArray.empty()) {
            TokenType trk = typeHolder->trKeywordArray[0];
            if (trk == KEYWORD_STRUCT) out << "struct ";
            else if (trk == KEYWORD_UNION) out << "union ";
            else if (trk == KEYWORD_ENUM) out << "enum ";
            
            if (!typeHolder->trBaseArray.empty()) {
                out << typeHolder->trBaseArray[0] << " ";
            }
        }
        
        // Print typedef names if present
        if (!typeHolder->tdNew.empty()) {
            out << typeHolder->tdNew[0] << " ";
        }
    } else {
        out << "NULL";
    }
    out << ")\n";
    out << indent << "  Expression:\n";
    if (expression) {
        expression->print(out, indent + "    ");
    } else {
        out << indent << "    NULL";
    }
}

// ============================================================================
// Sizeof Node Print Function
// ============================================================================

void SizeofNode::print(ofstream& out, const string& indent) const {
    if (isType) {
        out << indent << "Sizeof(";
        if (typeHolder) {
            // Print type qualifiers with their stars
            for (const auto& tq : typeHolder->typeQualifiersArray) {
                if (tq == KEYWORD_CONST) out << "const";
                else if (tq == KEYWORD_VOLATILE) out << "volatile";
                else if (tq == KEYWORD_RESTRICT) out << "restrict";
                
                for (const auto& starData : typeHolder->starDataArray) {
                    if (starData.typeBeforeStar == tq && starData.numOfStars > 0) {
                        for (int i = 0; i < starData.numOfStars; i++) {
                            out << "*";
                        }
                    }
                }
                out << " ";
            }
            
            // Print sign modifiers with their stars
            for (const auto& sm : typeHolder->signModifiersArray) {
                if (sm == KEYWORD_SIGNED) out << "signed";
                else if (sm == KEYWORD_UNSIGNED) out << "unsigned";
                
                for (const auto& starData : typeHolder->starDataArray) {
                    if (starData.typeBeforeStar == sm && starData.numOfStars > 0) {
                        for (int i = 0; i < starData.numOfStars; i++) {
                            out << "*";
                        }
                    }
                }
                out << " ";
            }
            
            // Print size modifiers with their stars
            for (const auto& szm : typeHolder->sizeModifiersArray) {
                if (szm == KEYWORD_SHORT) out << "short";
                else if (szm == KEYWORD_LONG) out << "long";
                
                for (const auto& starData : typeHolder->starDataArray) {
                    if (starData.typeBeforeStar == szm && starData.numOfStars > 0) {
                        for (int i = 0; i < starData.numOfStars; i++) {
                            out << "*";
                        }
                    }
                }
                out << " ";
            }
            
            // Print base type with stars
            if (!typeHolder->baseTypeArray.empty()) {
                TokenType bt = typeHolder->baseTypeArray[0];
                if (bt == KEYWORD_INT) out << "int";
                else if (bt == KEYWORD_CHAR) out << "char";
                else if (bt == KEYWORD_VOID) out << "void";
                else if (bt == KEYWORD_FLOAT) out << "float";
                else if (bt == KEYWORD_DOUBLE) out << "double";
                
                for (const auto& starData : typeHolder->starDataArray) {
                    if (starData.typeBeforeStar == bt && starData.numOfStars > 0) {
                        for (int i = 0; i < starData.numOfStars; i++) {
                            out << "*";
                        }
                    }
                }
                out << " ";
            }
            
            // Print struct/union/enum if present
            if (!typeHolder->trKeywordArray.empty()) {
                TokenType trk = typeHolder->trKeywordArray[0];
                if (trk == KEYWORD_STRUCT) out << "struct ";
                else if (trk == KEYWORD_UNION) out << "union ";
                else if (trk == KEYWORD_ENUM) out << "enum ";
                
                if (!typeHolder->trBaseArray.empty()) {
                    out << typeHolder->trBaseArray[0] << " ";
                }
            }
            
            // Print typedef names if present
            if (!typeHolder->tdNew.empty()) {
                out << typeHolder->tdNew[0] << " ";
            }
        } else {
            out << "NULL";
        }
        out << ")";
    } else {
        out << indent << "Sizeof(expr)\n";
        out << indent << "  Expression:\n";
        if (expression) {
            expression->print(out, indent + "    ");
        } else {
            out << indent << "    NULL";
        }
    }
}

// ============================================================================
// Block Expression Node Print Function
// ============================================================================

void BlockExpressionNode::print(ofstream& out, const string& indent) const {
    out << indent << "BlockExpression {\n";
    for (size_t i = 0; i < expressions.size(); i++) {
        if (expressions[i]) {
            // Try to cast to ExpressionNode or StatementNode
            ExpressionNode* exprNode = dynamic_cast<ExpressionNode*>(expressions[i]);
            if(exprNode) {
                exprNode->print(out, indent + "  ");
            } else {
                StatementNode* stmtNode = dynamic_cast<StatementNode*>(expressions[i]);
                if(stmtNode) {
                    stmtNode->print(out, indent + "  ");
                } else {
                    DeclarationNode* declNode = dynamic_cast<DeclarationNode*>(expressions[i]);
                    if(declNode) {
                        // Write to temp file to handle declaration printing
                        ofstream tempFile(".temp_block_decl.txt");
                        declNode->print(tempFile);
                        tempFile.close();

                        ifstream tempRead(".temp_block_decl.txt");
                        string line;
                        while(getline(tempRead, line)) {
                            if(!line.empty()) {
                                out << indent << "  " << line << "\n";
                            }
                        }
                        tempRead.close();
                        remove(".temp_block_decl.txt");
                    }
                }
            }
            if (i < expressions.size() - 1) {
                out << indent << ",";
            }
            out << "\n";
        }
    }
    out << indent << "}";
}

// ============================================================================
// Enum Block Expression Node Print Function
// ============================================================================

void EnumBlockExpressionNode::print(ofstream& out, const string& indent) const {
    out << indent << "EnumBlock {\n";
    for (size_t i = 0; i < components.size(); i++) {
        out << indent << "  " << components[i].name;
        if (components[i].value != nullptr) {
            out << " = ";
            components[i].value->print(out, "");
        }
        if (i < components.size() - 1) {
            out << ",";
        }
        out << "\n";
    }
    out << indent << "}";
}
