#include "StatementNode.h"
#include "ExpressionNode.h"
#include <fstream>

using namespace std;

void ExpressionStatementNode::print(ofstream& out) {
    out << "Expression Statement:\n";
    if (expression) {
        expression->print(out, "  ");
        out << "\n"; // Ensure newline after expression
    } else {
        out << "  NULL\n";
    }
}
