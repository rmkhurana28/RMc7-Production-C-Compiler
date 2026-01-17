#include "ExpressionNode.h"

// ExpressionNode implementations
// Add your function definitions here when needed



// constructor of binop node
BinaryOpNode::BinaryOpNode(TokenType opToken ,ExpressionNode* left ,ExpressionNode* right){
    this->binopToken = opToken;
    this->left = left;
    this->right = right;
}
