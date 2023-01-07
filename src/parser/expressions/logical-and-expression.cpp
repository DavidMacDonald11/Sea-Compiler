#include "parser/expressions/logical-and-expression.h"
#include "parser/expressions/logical-not-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new LogicalAndExpression(left, op, right);
}


LogicalAndExpression::LogicalAndExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* LogicalAndExpression::construct() {
    return BinaryOperation::construct(
        {"and"},
        LogicalNotExpression::construct,
        make
    );
}

Transpiler::Line LogicalAndExpression::transpile() {
    Transpiler::Line left = self.left.transpile();
    Transpiler::Line right = self.right.transpile();
    Transpiler::Line result = Transpiler::Line::resolve(left, right);

    return result.replace(fmt::format("({1}? {0} : {1})", 
        left.toString(), right.toString()));
}
