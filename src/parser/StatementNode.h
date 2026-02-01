#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include "ASTNode.h"

// Forward declaration
class ExpressionNode;

// ============================================================================
// Abstract base class for all statement nodes
// ============================================================================
class StatementNode : public ASTNode {
public:
    virtual ~StatementNode() {}
    
protected:
    StatementNode() {}
};

// ============================================================================
// STATEMENT NODE TYPES
// ============================================================================

class CompoundStatementNode : public StatementNode {
public:
    CompoundStatementNode() {}
    ~CompoundStatementNode() {}
};

class ExpressionStatementNode : public StatementNode {
public:
    ExpressionNode* expression;
    
    ExpressionStatementNode(ExpressionNode* expr) : expression(expr) {}
    ~ExpressionStatementNode() { delete expression; }
    void print(ofstream& out);
};

class IfStatementNode : public StatementNode {
public:
    IfStatementNode() {}
    ~IfStatementNode() {}
};

class WhileStatementNode : public StatementNode {
public:
    WhileStatementNode() {}
    ~WhileStatementNode() {}
};

class DoWhileStatementNode : public StatementNode {
public:
    DoWhileStatementNode() {}
    ~DoWhileStatementNode() {}
};

class ForStatementNode : public StatementNode {
public:
    ForStatementNode() {}
    ~ForStatementNode() {}
};

class SwitchStatementNode : public StatementNode {
public:
    SwitchStatementNode() {}
    ~SwitchStatementNode() {}
};

class CaseNode : public StatementNode {
public:
    CaseNode() {}
    ~CaseNode() {}
};

class DefaultNode : public StatementNode {
public:
    DefaultNode() {}
    ~DefaultNode() {}
};

class BreakStatementNode : public StatementNode {
public:
    BreakStatementNode() {}
    ~BreakStatementNode() {}
};

class ContinueStatementNode : public StatementNode {
public:
    ContinueStatementNode() {}
    ~ContinueStatementNode() {}
};

class ReturnStatementNode : public StatementNode {
public:
    ReturnStatementNode() {}
    ~ReturnStatementNode() {}
};

class GotoStatementNode : public StatementNode {
public:
    GotoStatementNode() {}
    ~GotoStatementNode() {}
};

class LabelStatementNode : public StatementNode {
public:
    LabelStatementNode() {}
    ~LabelStatementNode() {}
};

#endif
