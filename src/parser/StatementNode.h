#ifndef STATEMENTNODE_H
#define STATEMENTNODE_H

#include "ASTNode.h"
#include <vector>
#include <fstream>

using namespace std;

// Forward declarations
class ExpressionNode;
class BlockExpressionNode;
class DeclarationNode;

// ============================================================================
// Abstract base class for all statement nodes
// ============================================================================
class StatementNode : public ASTNode {
public:
    virtual ~StatementNode() {}
    virtual void print(ofstream& out, const string& indent = "") {
        out << indent << "Generic Statement (print not implemented)\n";
    }
    
protected:
    StatementNode() {}
};

// ============================================================================
// STATEMENT NODE TYPES
// ============================================================================

class ExpressionStatementNode : public StatementNode {
public:
    ExpressionNode* expression;
    
    ExpressionStatementNode(ExpressionNode* expr) : expression(expr) {}
    ~ExpressionStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class IfStatementNode : public StatementNode {
public:
    ExpressionNode* ifCond;
    BlockExpressionNode* ifBlock;
    bool elsePresent;
    BlockExpressionNode* elseBlock;

    IfStatementNode(ExpressionNode* cond, BlockExpressionNode* ifBlk, bool hasElse, BlockExpressionNode* elseBlk);
    ~IfStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class WhileStatementNode : public StatementNode {
public:
    ExpressionNode* whileCond;
    BlockExpressionNode* whileBlock;

    WhileStatementNode(ExpressionNode* cond, BlockExpressionNode* blk);
    ~WhileStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class DoWhileStatementNode : public StatementNode {
public:
    ExpressionNode* doWhileCond;
    BlockExpressionNode* doWhileBlock;

    DoWhileStatementNode(ExpressionNode* cond, BlockExpressionNode* blk);
    ~DoWhileStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class ForStatementNode : public StatementNode {
public:
    ASTNode* forInit;
    ExpressionNode* forCond;
    ExpressionNode* forIncr;
    BlockExpressionNode* forBlock;

    ForStatementNode(ASTNode* init, ExpressionNode* cond, ExpressionNode* incr, BlockExpressionNode* blk);
    ~ForStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class SwitchStatementNode : public StatementNode {
public:
    ExpressionNode* switchCond;
    BlockExpressionNode* switchBlock;

    SwitchStatementNode(ExpressionNode* cond, BlockExpressionNode* blk);
    ~SwitchStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class CaseLabelNode : public StatementNode {
public:
    ExpressionNode* caseExpr;

    CaseLabelNode(ExpressionNode* expr);
    ~CaseLabelNode();
    void print(ofstream& out, const string& indent = "");
};

class DefaultLabelNode : public StatementNode {
public:
    DefaultLabelNode() {}
    ~DefaultLabelNode() {}
    void print(ofstream& out, const string& indent = "");
};

class BreakStatementNode : public StatementNode {
public:
    BreakStatementNode() {}
    ~BreakStatementNode() {}
    void print(ofstream& out, const string& indent = "");
};

class ContinueStatementNode : public StatementNode {
public:
    ContinueStatementNode() {}
    ~ContinueStatementNode() {}
    void print(ofstream& out, const string& indent = "");
};

class ReturnStatementNode : public StatementNode {
public:
    ExpressionNode* retExpr;

    ReturnStatementNode(ExpressionNode* expr);
    ~ReturnStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class GotoStatementNode : public StatementNode {
public:
    ExpressionNode* gotoId;

    GotoStatementNode(ExpressionNode* id);
    ~GotoStatementNode();
    void print(ofstream& out, const string& indent = "");
};

class LabelStatementNode : public StatementNode {
public:
    ExpressionNode* labelId;

    LabelStatementNode(ExpressionNode* id);
    ~LabelStatementNode();
    void print(ofstream& out, const string& indent = "");
};

#endif
