#include "StatementNode.h"
#include "ExpressionNode.h"
#include "DeclarationNode.h"

// StatementNode implementations

// ExpressionStatementNode destructor
ExpressionStatementNode::~ExpressionStatementNode() {
    delete expression;
}

// IfStatementNode constructor and destructor
IfStatementNode::IfStatementNode(ExpressionNode* cond, BlockExpressionNode* ifBlk, bool hasElse, BlockExpressionNode* elseBlk)
    : ifCond(cond), ifBlock(ifBlk), elsePresent(hasElse), elseBlock(elseBlk) {}

IfStatementNode::~IfStatementNode() {
    delete ifCond;
    delete ifBlock;
    if(elseBlock) delete elseBlock;
}

// WhileStatementNode constructor and destructor
WhileStatementNode::WhileStatementNode(ExpressionNode* cond, BlockExpressionNode* blk)
    : whileCond(cond), whileBlock(blk) {}

WhileStatementNode::~WhileStatementNode() {
    delete whileCond;
    delete whileBlock;
}

// DoWhileStatementNode constructor and destructor
DoWhileStatementNode::DoWhileStatementNode(ExpressionNode* cond, BlockExpressionNode* blk)
    : doWhileCond(cond), doWhileBlock(blk) {}

DoWhileStatementNode::~DoWhileStatementNode() {
    delete doWhileCond;
    delete doWhileBlock;
}

// ForStatementNode constructor and destructor
ForStatementNode::ForStatementNode(vector<ASTNode*> inits, ExpressionNode* cond, ExpressionNode* incr, BlockExpressionNode* blk)
    : forInits(inits), forCond(cond), forIncr(incr), forBlock(blk) {}

ForStatementNode::~ForStatementNode() {
    for(ASTNode* node : forInits) delete node;
    if(forCond) delete forCond;
    if(forIncr) delete forIncr;
    delete forBlock;
}

// SwitchStatementNode constructor and destructor
SwitchStatementNode::SwitchStatementNode(ExpressionNode* cond, BlockExpressionNode* blk)
    : switchCond(cond), switchBlock(blk) {}

SwitchStatementNode::~SwitchStatementNode() {
    delete switchCond;
    delete switchBlock;
}

// CaseLabelNode constructor and destructor
CaseLabelNode::CaseLabelNode(ExpressionNode* expr) : caseExpr(expr) {}

CaseLabelNode::~CaseLabelNode() {
    delete caseExpr;
}

// ReturnStatementNode constructor and destructor
ReturnStatementNode::ReturnStatementNode(ExpressionNode* expr) : retExpr(expr) {}

ReturnStatementNode::~ReturnStatementNode() {
    if(retExpr) delete retExpr;
}

// GotoStatementNode constructor and destructor
GotoStatementNode::GotoStatementNode(ExpressionNode* id) : gotoId(id) {}

GotoStatementNode::~GotoStatementNode() {
    delete gotoId;
}

// LabelStatementNode constructor and destructor
LabelStatementNode::LabelStatementNode(ExpressionNode* id) : labelId(id) {}

LabelStatementNode::~LabelStatementNode() {
    delete labelId;
}

