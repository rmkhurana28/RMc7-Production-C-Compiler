#ifndef DECLARATIONNODE_H
#define DECLARATIONNODE_H

#include "ASTNode.h"
#include <vector>
#include <fstream>
#include "ExpressionNode.h"
#include "Helper.h"
using namespace std;

// ============================================================================
// Abstract base class for all declaration nodes
// ============================================================================
class DeclarationNode : public ASTNode {
public:
    virtual ~DeclarationNode() {}
    virtual void print(ofstream& out) = 0; // Pure virtual print function
    
protected:
    DeclarationNode() {}
};

// ============================================================================
// DECLARATION NODE TYPES
// ============================================================================

class dataTypeHolder;
class varNameHolder;
class ParameterNode;

class Parser;  // Forward declaration

class VariableDeclarationNode : public DeclarationNode {
private:
    dataTypeHolder varDeclType; // to store the decl type of the var
    varNameHolder varName; // to store the var name of the var
    
    bool isInit; // true if init, else false
    ExpressionNode* initExpr; // init expression

    bool isArray; // true if array, else false
    short unsigned arrDim; // dimensions of the array

public:
    VariableDeclarationNode(dataTypeHolder* type , varNameHolder* name , bool isInitBool , ExpressionNode* expr , bool isArr , short unsigned arrDimensions);
    ~VariableDeclarationNode() {}
    void print(ofstream& out) override;
    void printParameters(ofstream& out);
};

class FunctionDeclarationNode : public DeclarationNode {
private:
    dataTypeHolder funcDeclType; // to store the return type of the func
    varNameHolder funcName; // to store the function name

    // storage to store parameters
    vector<ParameterNode> paramsArray; // array to store all the params

    bool isVariadic; // true if function accepts variable parameters [like int param1 , ...]

public:
    FunctionDeclarationNode(dataTypeHolder* retType, varNameHolder* name, vector<ParameterNode> params, bool isVar);
    ~FunctionDeclarationNode() {}
    void print(ofstream& out) override;
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
    dataTypeHolder paramType;
    varNameHolder paramName;   

    ParameterNode(dataTypeHolder* type, varNameHolder* name);
    ParameterNode& operator=(const ParameterNode& other);

    static vector<ParameterNode> evaluateParams(Parser& parser);

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
    
    void printAST(ofstream& out);
};

class ArrayDeclaratorNode : public ASTNode {
public:
    ArrayDeclaratorNode() {}
    ~ArrayDeclaratorNode() {}
};

#endif
