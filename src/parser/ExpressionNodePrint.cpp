#include "ExpressionNode.h"
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
    out << indent << "Cast(" << targetTypeName << ")\n";
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
        out << indent << "Sizeof(type: " << typeName << ")";
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
            expressions[i]->print(out, indent + "  ");
            if (i < expressions.size() - 1) {
                out << ",";
            }
            out << "\n";
        }
    }
    out << indent << "}";
}
