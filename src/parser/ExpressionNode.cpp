#include "ExpressionNode.h"
#include "Helper.h"

// Literal node constructors
IntLiteralNode::IntLiteralNode(string value) {
    this->numLiteralValue = value;
}

FloatLiteralNode::FloatLiteralNode(string value) {
    this->floatLiteralValue = value;
}

DoubleLiteralNode::DoubleLiteralNode(string value) {
    this->doubleLiteralValue = value;
}

CharLiteralNode::CharLiteralNode(string value) {
    this->charLiteralValue = value;
}

StringLiteralNode::StringLiteralNode(string value) {
    this->stringLiteralValue = value;
}

BoolLiteralNode::BoolLiteralNode(bool value) {
    this->boolLiteralValue = value;
}

// IdentifierNode constructor
IdentifierNode::IdentifierNode(string name) {
    this->identifierName = name;
}

// BinaryOpNode constructor and destructor
BinaryOpNode::BinaryOpNode(TokenType opToken, ExpressionNode* left, ExpressionNode* right) {
    this->binOpType = opToken;
    this->left = left;
    this->right = right;
}

BinaryOpNode::~BinaryOpNode() {
    delete left;
    delete right;
}

// UnaryOpNode constructor and destructor
UnaryOpNode::UnaryOpNode(TokenType opToken, ExpressionNode* operand, bool isPrefix) {
    this->unOpType = opToken;
    this->operand = operand;
    this->isPrefix = isPrefix;
}

UnaryOpNode::~UnaryOpNode() {
    delete operand;
}

// TernaryOpNode constructor and destructor
TernaryOpNode::TernaryOpNode(ExpressionNode* cond, ExpressionNode* trueExpr, ExpressionNode* falseExpr) {
    this->condition = cond;
    this->trueExpr = trueExpr;
    this->falseExpr = falseExpr;
}

TernaryOpNode::~TernaryOpNode() {
    delete condition;
    delete trueExpr;
    delete falseExpr;
}

// ArrayAccessNode constructor and destructor
ArrayAccessNode::ArrayAccessNode(ExpressionNode* array, ExpressionNode* index) {
    this->arrayExpr = array;
    this->indexExpr = index;
}

ArrayAccessNode::~ArrayAccessNode() {
    delete arrayExpr;
    delete indexExpr;
}

// MemberAccessNode constructor and destructor
MemberAccessNode::MemberAccessNode(ExpressionNode* obj, string member) {
    this->objectExpr = obj;
    this->memberName = member;
}

MemberAccessNode::~MemberAccessNode() {
    delete objectExpr;
}

// PointerMemberAccessNode constructor and destructor
PointerMemberAccessNode::PointerMemberAccessNode(ExpressionNode* ptr, string member) {
    this->pointerExpr = ptr;
    this->memberName = member;
}

PointerMemberAccessNode::~PointerMemberAccessNode() {
    delete pointerExpr;
}

// FunctionCallNode constructor and destructor
FunctionCallNode::FunctionCallNode(ExpressionNode* func, vector<ExpressionNode*> args) {
    this->functionExpr = func;
    this->arguments = args;
}

FunctionCallNode::~FunctionCallNode() {
    delete functionExpr;
    for (ExpressionNode* arg : arguments) {
        delete arg;
    }
}

// CastNode constructor and destructor
CastNode::CastNode(dataTypeHolder* typeData, ExpressionNode* expr) {
    this->typeHolder = typeData;
    this->expression = expr;
}

CastNode::~CastNode() {
    delete typeHolder;
    delete expression;
}

// SizeofNode constructors and destructor
SizeofNode::SizeofNode(dataTypeHolder* typeData) {
    this->isType = true;
    this->typeHolder = typeData;
    this->expression = nullptr;
}

SizeofNode::SizeofNode(ExpressionNode* expr) {
    this->isType = false;
    this->typeHolder = nullptr;
    this->expression = expr;
}

SizeofNode::~SizeofNode() {
    if (typeHolder != nullptr) {
        delete typeHolder;
    }
    if (expression != nullptr) {
        delete expression;
    }
}

// BlockExpressionNode constructor and destructor
BlockExpressionNode::BlockExpressionNode(vector<ASTNode*> exprs) {
    this->expressions = exprs;
}

BlockExpressionNode::~BlockExpressionNode() {
    for (ASTNode* expr : expressions) {
        delete expr;
    }
}

// enumComponentHolder constructor and destructor
enumComponentHolder::enumComponentHolder(string enumName, ExpressionNode* enumValue) {
    this->name = enumName;
    this->value = enumValue;
}

enumComponentHolder::~enumComponentHolder() {
    if (value != nullptr) {
        delete value;
    }
}

enumComponentHolder::enumComponentHolder(enumComponentHolder&& other) noexcept
    : name(std::move(other.name)), value(other.value) {
    other.value = nullptr;
}

enumComponentHolder& enumComponentHolder::operator=(enumComponentHolder&& other) noexcept {
    if (this != &other) {
        delete value;
        name = std::move(other.name);
        value = other.value;
        other.value = nullptr;
    }
    return *this;
}

// EnumBlockExpressionNode constructor and destructor
EnumBlockExpressionNode::EnumBlockExpressionNode(vector<enumComponentHolder> enums) {
    this->components = std::move(enums);
}

EnumBlockExpressionNode::~EnumBlockExpressionNode() {
    // vector destructor handles calling ~enumComponentHolder() on each element
}
