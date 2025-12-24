#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H

#include "ASTNode.h"
#include "../lexer/Token.h"

// ============================================================================
// Abstract base class for all expression nodes
// ============================================================================
class ExpressionNode : public ASTNode {
public:
    virtual ~ExpressionNode() {}
    
protected:
    ExpressionNode() {}
};

// ============================================================================
// LITERAL NODES
// ============================================================================

// Abstract base for all literal nodes
class LiteralNode : public ExpressionNode {
public:
    virtual ~LiteralNode() {}
    
protected:
    LiteralNode() {}
};

class IntLiteralNode : public LiteralNode {
private:
    int numLiteralValue;

public:
    IntLiteralNode(int value) {
        this->numLiteralValue = value;
    }
    ~IntLiteralNode() {}
};

class FloatLiteralNode : public LiteralNode {
public:
    FloatLiteralNode() {}
    ~FloatLiteralNode() {}
};

class DoubleLiteralNode : public LiteralNode {
public:
    DoubleLiteralNode() {}
    ~DoubleLiteralNode() {}
};

class CharLiteralNode : public LiteralNode {
public:
    CharLiteralNode() {}
    ~CharLiteralNode() {}
};

class StringLiteralNode : public LiteralNode {
public:
    StringLiteralNode() {}
    ~StringLiteralNode() {}
};

class BoolLiteralNode : public LiteralNode {
public:
    BoolLiteralNode() {}
    ~BoolLiteralNode() {}
};

// ============================================================================
// OTHER EXPRESSION NODES
// ============================================================================

class IdentifierNode : public ExpressionNode {
public:
    IdentifierNode() {}
    ~IdentifierNode() {}
};

class BinaryOpNode : public ExpressionNode {
private:
    TokenType binopToken; // to store token type of operator
    ExpressionNode* left; // to store left AST expression node
    ExpressionNode* right; // to store right AST expression node    

public:
    BinaryOpNode(TokenType opToken ,ExpressionNode* left ,ExpressionNode* right);
    ~BinaryOpNode() {}
};

class UnaryOpNode : public ExpressionNode {
public:
    UnaryOpNode() {}
    ~UnaryOpNode() {}
};

class TernaryOpNode : public ExpressionNode {
public:
    TernaryOpNode() {}
    ~TernaryOpNode() {}
};

class AssignmentNode : public ExpressionNode {
private:
    TokenType assignOpToken; // store assignment operator
    ExpressionNode* var; // store left side of assignment
    ExpressionNode* expr; // store right side experssion

public:
    AssignmentNode(TokenType opToken ,ExpressionNode* var ,ExpressionNode* expr){
        this->assignOpToken = opToken;
        this->var = var;
        this->expr = expr;
    }
    ~AssignmentNode() {}
};

class FunctionCallNode : public ExpressionNode {
public:
    FunctionCallNode() {}
    ~FunctionCallNode() {}
};

class ArrayAccessNode : public ExpressionNode {
public:
    ArrayAccessNode() {}
    ~ArrayAccessNode() {}
};

class MemberAccessNode : public ExpressionNode {
public:
    MemberAccessNode() {}
    ~MemberAccessNode() {}
};

class PointerMemberAccessNode : public ExpressionNode {
public:
    PointerMemberAccessNode() {}
    ~PointerMemberAccessNode() {}
};

class CastNode : public ExpressionNode {
public:
    CastNode() {}
    ~CastNode() {}
};

class SizeofNode : public ExpressionNode {
public:
    SizeofNode() {}
    ~SizeofNode() {}
};

#endif
