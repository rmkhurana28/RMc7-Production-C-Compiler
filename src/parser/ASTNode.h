#ifndef ASTNODE_H
#define ASTNODE_H

#include <string>
#include <vector>
using namespace std;

// Base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() {}
    
    // Add your member variables here (line, column, etc.)
    
protected:
    ASTNode() {}
};

#endif
