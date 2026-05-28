#ifndef EXPRESSIONNODE_H
#define EXPRESSIONNODE_H

#include <string>
#include <vector>
#include <fstream>
#include "ASTNode.h"
#include "../lexer/Token.h"

using namespace std;

// Forward declaration
class dataTypeHolder;

// ============================================================================
// Abstract base class for all expression nodes
// ============================================================================
class ExpressionNode : public ASTNode {
public:
    virtual ~ExpressionNode() {}
    virtual void print(ofstream& out, const string& indent = "") const = 0;
    
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
    string numLiteralValue;

public:
    IntLiteralNode(string value);
    ~IntLiteralNode() {}
    void print(ofstream& out, const string& indent = "") const override;
};

class FloatLiteralNode : public LiteralNode {
private:
    string floatLiteralValue;

public:
    FloatLiteralNode(string value);
    ~FloatLiteralNode() {}
    void print(ofstream& out, const string& indent = "") const override;
};

class DoubleLiteralNode : public LiteralNode {
private:
    string doubleLiteralValue;

public:
    DoubleLiteralNode(string value);
    void print(ofstream& out, const string& indent = "") const override;
    ~DoubleLiteralNode() {}
};

class CharLiteralNode : public LiteralNode {
private:
    string charLiteralValue;

public:
    CharLiteralNode(string value);
    void print(ofstream& out, const string& indent = "") const override;
    ~CharLiteralNode() {}
};

class StringLiteralNode : public LiteralNode {
private:
    string stringLiteralValue;

public:
    StringLiteralNode(string value);
    void print(ofstream& out, const string& indent = "") const override;
    ~StringLiteralNode() {}
};

class BoolLiteralNode : public LiteralNode {
private:
    bool boolLiteralValue;

public:
    void print(ofstream& out, const string& indent = "") const override;
    BoolLiteralNode(bool value);
    ~BoolLiteralNode() {}
};

// ============================================================================
// OTHER EXPRESSION NODES
// ============================================================================

class IdentifierNode : public ExpressionNode {
private:
    string identifierName;

public:
    void print(ofstream& out, const string& indent = "") const override;
    IdentifierNode(string name);
    ~IdentifierNode() {}
};

class BinaryOpNode : public ExpressionNode {
private:
    TokenType binOpType; // to store token type of binary operator
    ExpressionNode* left; // to store left AST expression node
    ExpressionNode* right; // to store right AST expression node    

    void print(ofstream& out, const string& indent = "") const override;
public:
    BinaryOpNode(TokenType opToken ,ExpressionNode* left ,ExpressionNode* right);
    ~BinaryOpNode();
};

class UnaryOpNode : public ExpressionNode {
private:
    TokenType unOpType; // to store token type of unary operator
    ExpressionNode* operand; // to store the operand AST expression node
    bool isPrefix; // true for ++x, false for x++

    void print(ofstream& out, const string& indent = "") const override;
public:
    UnaryOpNode(TokenType opToken, ExpressionNode* operand, bool isPrefix);
    ~UnaryOpNode();
};

class TernaryOpNode : public ExpressionNode {
private:
    ExpressionNode* condition; // to store the condition of ternary operator
    ExpressionNode* trueExpr; // to store the value if condition is true
    ExpressionNode* falseExpr; // to store the value if condition is false

    void print(ofstream& out, const string& indent = "") const override;
public:
    TernaryOpNode(ExpressionNode* cond, ExpressionNode* trueExpr, ExpressionNode* falseExpr);
    ~TernaryOpNode();
};

class FunctionCallNode : public ExpressionNode {
private:
    ExpressionNode* functionExpr; // to store function expression (can be identifier, member access, etc.)
    vector<ExpressionNode*> arguments; // to store argument expressions
    void print(ofstream& out, const string& indent = "") const override;

public:
    FunctionCallNode(ExpressionNode* func, vector<ExpressionNode*> args);
    ~FunctionCallNode();
};

class ArrayAccessNode : public ExpressionNode {
private:
    ExpressionNode* arrayExpr; // to store array expression (can be identifier, member access, etc.)
    ExpressionNode* indexExpr; // to store index expression
    void print(ofstream& out, const string& indent = "") const override;

public:
    ArrayAccessNode(ExpressionNode* array, ExpressionNode* index);
    ~ArrayAccessNode();
};

class MemberAccessNode : public ExpressionNode {
private:
    ExpressionNode* objectExpr; // to store the struct/union expression (can be nested!)
    void print(ofstream& out, const string& indent = "") const override;
    string memberName; // to store the member name (identifier, NOT expression)

public:
    MemberAccessNode(ExpressionNode* obj, string member);
    ~MemberAccessNode();
};

class PointerMemberAccessNode : public ExpressionNode {
private:
    ExpressionNode* pointerExpr; // to store the pointer expression (can be nested!)
    void print(ofstream& out, const string& indent = "") const override;
    string memberName; // to store the member name (identifier, NOT expression)

public:
    PointerMemberAccessNode(ExpressionNode* ptr, string member);
    ~PointerMemberAccessNode();
};

class CastNode : public ExpressionNode {
private:
    void print(ofstream& out, const string& indent = "") const override;
    dataTypeHolder* typeHolder; // to store the type information
    ExpressionNode* expression; // to store the expression being cast

public:
    CastNode(dataTypeHolder* typeData, ExpressionNode* expr);
    ~CastNode();
};

class SizeofNode : public ExpressionNode {
private:
    bool isType; // true if sizeof(type), false if sizeof(expr)
    dataTypeHolder* typeHolder; // to store type information if isType=true
    void print(ofstream& out, const string& indent = "") const override;
    ExpressionNode* expression; // to store expression if isType=false

public:
    SizeofNode(dataTypeHolder* typeData); // for sizeof(type)
    SizeofNode(ExpressionNode* expr); // for sizeof(expr)
    ~SizeofNode();
};

class BlockExpressionNode : public ExpressionNode {
public:
    vector<ASTNode*> expressions; // to store block of expressions/statements (e.g., {1, 2, 3} or compound statements)
    void print(ofstream& out, const string& indent = "") const override;

    BlockExpressionNode(vector<ASTNode*> exprs);
    ~BlockExpressionNode();
};

// ============================================================================
// ENUM SUPPORT
// ============================================================================

class enumComponentHolder {
public:
    string name; // enumerator name
    ExpressionNode* value; // optional value expression (nullptr if not assigned)

    enumComponentHolder(string enumName, ExpressionNode* enumValue = nullptr);
    ~enumComponentHolder();

    // Move semantics (transfer ownership of value pointer)
    enumComponentHolder(enumComponentHolder&& other) noexcept;
    enumComponentHolder& operator=(enumComponentHolder&& other) noexcept;

    // Disable copy (raw pointer ownership)
    enumComponentHolder(const enumComponentHolder&) = delete;
    enumComponentHolder& operator=(const enumComponentHolder&) = delete;
};

class EnumBlockExpressionNode : public ExpressionNode {
public:
    vector<enumComponentHolder> components; // to store list of enumerators
    void print(ofstream& out, const string& indent = "") const override;

    EnumBlockExpressionNode(vector<enumComponentHolder> enums);
    ~EnumBlockExpressionNode();
};



#endif
