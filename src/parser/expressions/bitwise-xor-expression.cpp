#include "parser/expressions/bitwise-and-expression.h"
#include "parser/expressions/bitwise-xor-expression.h"
#include "parser/node.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new BitwiseXorExpression(left, op, right);
}


BitwiseXorExpression::BitwiseXorExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* BitwiseXorExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        {"$"},
        BitwiseAndExpression::construct,
        make
    );
}

Transpiler::Line BitwiseXorExpression::transpile(Transpiler& transpiler) {
    return transpileBinary(transpiler, "^");
}
