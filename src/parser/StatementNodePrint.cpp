#include "StatementNode.h"
#include "ExpressionNode.h"
#include "DeclarationNode.h"
#include <fstream>

using namespace std;

void ExpressionStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Expression Statement:\n";
    if (expression) {
        expression->print(out, indent + "  ");
        out << "\n"; // Ensure newline after expression
    } else {
        out << indent << "  NULL\n";
    }
}

void IfStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "If Statement:\n";
    out << indent << "  Condition:\n";
    if (ifCond) {
        ifCond->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  If Block:\n";
    if (ifBlock) {
        if (ifBlock->expressions.empty()) {
            out << indent << "    (empty block)\n";
        } else {
            for (size_t i = 0; i < ifBlock->expressions.size(); i++) {
                if (ifBlock->expressions[i]) {
                    StatementNode* stmt = dynamic_cast<StatementNode*>(ifBlock->expressions[i]);
                    if (stmt) {
                        stmt->print(out, indent + "    ");
                    } else {
                        out << indent << "    [Non-statement node]\n";
                    }
                } else {
                    out << indent << "    [NULL node]\n";
                }
            }
        }
    } else {
        out << indent << "    (null block)\n";
    }
    if (elsePresent) {
        out << indent << "  Else Block:\n";
        if (elseBlock) {
            if (elseBlock->expressions.empty()) {
                out << indent << "    (empty block)\n";
            } else {
                for (size_t i = 0; i < elseBlock->expressions.size(); i++) {
                    if (elseBlock->expressions[i]) {
                        StatementNode* stmt = dynamic_cast<StatementNode*>(elseBlock->expressions[i]);
                        if (stmt) {
                            stmt->print(out, indent + "    ");
                        } else {
                            out << indent << "    [Non-statement node]\n";
                        }
                    } else {
                        out << indent << "    [NULL node]\n";
                    }
                }
            }
        } else {
            out << indent << "    (null block)\n";
        }
    }
}

void WhileStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "While Statement:\n";
    out << indent << "  Condition:\n";
    if (whileCond) {
        whileCond->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Block:\n";
    if (whileBlock && !whileBlock->expressions.empty()) {
        for (size_t i = 0; i < whileBlock->expressions.size(); i++) {
            if (whileBlock->expressions[i]) {
                StatementNode* stmt = dynamic_cast<StatementNode*>(whileBlock->expressions[i]);
                if (stmt) {
                    stmt->print(out, indent + "    ");
                } else {
                    out << indent << "    [Non-statement node]\n";
                }
            }
        }
    } else {
        out << indent << "    (empty)\n";
    }
}

void DoWhileStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Do-While Statement:\n";
    out << indent << "  Block:\n";
    if (doWhileBlock && !doWhileBlock->expressions.empty()) {
        for (size_t i = 0; i < doWhileBlock->expressions.size(); i++) {
            if (doWhileBlock->expressions[i]) {
                StatementNode* stmt = dynamic_cast<StatementNode*>(doWhileBlock->expressions[i]);
                if (stmt) {
                    stmt->print(out, indent + "    ");
                } else {
                    out << indent << "    [Non-statement node]\n";
                }
            }
        }
    } else {
        out << indent << "    (empty)\n";
    }
    out << indent << "  Condition:\n";
    if (doWhileCond) {
        doWhileCond->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
}

void ForStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "For Statement:\n";
    out << indent << "  Init:\n";
    if (forInit) {
        // Try casting to different node types
        ExpressionStatementNode* exprStmt = dynamic_cast<ExpressionStatementNode*>(forInit);
        StatementNode* stmt = dynamic_cast<StatementNode*>(forInit);
        DeclarationNode* decl = dynamic_cast<DeclarationNode*>(forInit);
        
        if (exprStmt) {
            // It's an expression statement (e.g., i = 0;)
            exprStmt->print(out, indent + "    ");
        } else if (stmt) {
            // It's some other statement type
            stmt->print(out, indent + "    ");
        } else if (decl) {
            // It's a declaration (e.g., int i = 0;)
            decl->print(out);
        } else {
            out << indent << "    [Unknown node type]\n";
        }
    } else {
        out << indent << "    (empty)\n";
    }
    out << indent << "  Condition:\n";
    if (forCond) {
        forCond->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Increment:\n";
    if (forIncr) {
        forIncr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Block:\n";
    if (forBlock && !forBlock->expressions.empty()) {
        for (size_t i = 0; i < forBlock->expressions.size(); i++) {
            if (forBlock->expressions[i]) {
                StatementNode* stmt = dynamic_cast<StatementNode*>(forBlock->expressions[i]);
                if (stmt) {
                    stmt->print(out, indent + "    ");
                } else {
                    out << indent << "    [Non-statement node]\n";
                }
            }
        }
    } else {
        out << indent << "    (empty)\n";
    }
}

void ReturnStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Return Statement:\n";
    if (retExpr) {
        out << indent << "  Return value:\n";
        retExpr->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "  (void return)\n";
    }
}

void BreakStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Break Statement\n";
}

void ContinueStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Continue Statement\n";
}

void GotoStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Goto Statement:\n";
    out << indent << "  Label: ";
    if (gotoId) {
        gotoId->print(out, "");
    } else {
        out << "NULL";
    }
    out << "\n";
}

void LabelStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Label Statement:\n";
    out << indent << "  Label: ";
    if (labelId) {
        labelId->print(out, "");
    } else {
        out << "NULL";
    }
    out << "\n";
}

void SwitchStatementNode::print(ofstream& out, const string& indent) {
    out << indent << "Switch Statement:\n";
    out << indent << "  Condition:\n";
    if (switchCond) {
        switchCond->print(out, indent + "    ");
        out << "\n";
    } else {
        out << indent << "    NULL\n";
    }
    out << indent << "  Block:\n";
    if (switchBlock && !switchBlock->expressions.empty()) {
        for (size_t i = 0; i < switchBlock->expressions.size(); i++) {
            if (switchBlock->expressions[i]) {
                StatementNode* stmt = dynamic_cast<StatementNode*>(switchBlock->expressions[i]);
                if (stmt) {
                    stmt->print(out, indent + "    ");
                } else {
                    out << indent << "    [Non-statement node]\n";
                }
            }
        }
    } else {
        out << indent << "    (empty)\n";
    }
}

void CaseLabelNode::print(ofstream& out, const string& indent) {
    out << indent << "Case Label:\n";
    if (caseExpr) {
        out << indent << "  Expression: ";
        caseExpr->print(out, "");
        out << "\n";
    } else {
        out << indent << "  NULL\n";
    }
}

void DefaultLabelNode::print(ofstream& out, const string& indent) {
    out << indent << "Default Label\n";
}
