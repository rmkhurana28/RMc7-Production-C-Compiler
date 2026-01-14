#ifndef DECLARATIONNODE_H
#define DECLARATIONNODE_H

#include "ASTNode.h"
#include <vector>
using namespace std;

// ============================================================================
// Abstract base class for all declaration nodes
// ============================================================================
class DeclarationNode : public ASTNode {
public:
    virtual ~DeclarationNode() {}
    
protected:
    DeclarationNode() {}
};

// ============================================================================
// DECLARATION NODE TYPES
// ============================================================================

class VariableDeclarationNode : public DeclarationNode {
private:
    dataTypeHolder varDeclType; // to store the decl type of the var
    varNameHolder varName; // to store the var name of the var
    bool isInit; // true if init, else false
    ExpressionNode initExpr; // init expression

public:
    VariableDeclarationNode();
    ~VariableDeclarationNode() {}
};

class FunctionDeclarationNode : public DeclarationNode {
public:
    FunctionDeclarationNode() {}
    ~FunctionDeclarationNode() {}
};

class FunctionDefinitionNode : public DeclarationNode {
public:
    FunctionDefinitionNode() {}
    ~FunctionDefinitionNode() {}
};

class StructDeclarationNode : public DeclarationNode {
public:
    StructDeclarationNode() {}
    ~StructDeclarationNode() {}
};

class UnionDeclarationNode : public DeclarationNode {
public:
    UnionDeclarationNode() {}
    ~UnionDeclarationNode() {}
};

class EnumDeclarationNode : public DeclarationNode {
public:
    EnumDeclarationNode() {}
    ~EnumDeclarationNode() {}
};

class TypedefDeclarationNode : public DeclarationNode {
public:
    TypedefDeclarationNode() {}
    ~TypedefDeclarationNode() {}
};

// ============================================================================
// SUPPORTING NODES
// ============================================================================

class ParameterNode : public ASTNode {
public:
    ParameterNode() {}
    ~ParameterNode() {}
};

class ProgramNode : public ASTNode {
private:
    vector<DeclarationNode*> declarations; // All top-level declarations (functions, variables, structs, etc.)

public:
    ProgramNode(vector<DeclarationNode*> declarations) {
        this->declarations = declarations;
    }
    
    ~ProgramNode() {
        // Clean up all declarations
        for (DeclarationNode* decl : declarations) {
            delete decl;
        }
    }
    
    vector<DeclarationNode*> getDeclarations() {
        return declarations;
    }
};

class ArrayDeclaratorNode : public ASTNode {
public:
    ArrayDeclaratorNode() {}
    ~ArrayDeclaratorNode() {}
};

#endif
