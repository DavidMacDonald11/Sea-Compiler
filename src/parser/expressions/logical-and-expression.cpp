#include "parser/expressions/logical-and-expression.h"
#include "parser/expressions/logical-not-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new LogicalAndExpression(left, op, right);
}


LogicalAndExpression::LogicalAndExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* LogicalAndExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        {"and"},
        LogicalNotExpression::construct,
        make
    );
}

Transpiler::Line LogicalAndExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line left = self.left.transpile(transpiler);
    Transpiler::Line right = self.right.transpile(transpiler);
    Transpiler::Line result = Transpiler::Line::resolve(left, right);

    return result.replace(fmt::format("({1}? {0} : {1})", 
        left.toString(), right.toString()));
}
