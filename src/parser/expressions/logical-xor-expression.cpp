#include "parser/expressions/logical-and-expression.h"
#include "parser/expressions/logical-xor-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new LogicalXorExpression(left, op, right);
}


LogicalXorExpression::LogicalXorExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* LogicalXorExpression::construct() {
    return BinaryOperation::construct(
        {"xor"},
        LogicalAndExpression::construct,
        make
    );
}

Transpiler::Line LogicalXorExpression::transpile() {
    Transpiler::Line left = self.left.transpile();
    Transpiler::Line right = self.right.transpile();
    Transpiler::Line result = Transpiler::Line::resolve(left, right);

    return result.replace(fmt::format("(({0} && {1})? 0 : {0}? {0} : {1})", 
        left.toString(), right.toString()));
}
