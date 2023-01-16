#include "parser/expressions/cast-expression.h"
#include "parser/expressions/comparative-expression.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"

static Node* make(Node& left, Token& op, Node& right) {
    return new ComparativeExpression(left, op, right);
}


ComparativeExpression::ComparativeExpression(Node& left, Token& op, Node& right)
: BinaryOperation(left, op, right) {}

Node* ComparativeExpression::construct(Parser& parser) {
    return BinaryOperation::construct(
        parser,
        Token::COMPARE_OPS,
        CastExpression::construct,
        make
    );
}

Transpiler::Line ComparativeExpression::transpile(Transpiler& transpiler) {
    Transpiler::Line left = self.left.transpile(transpiler);
    Transpiler::Line right = self.right.transpile(transpiler);
    Transpiler::Line result = Transpiler::Line::resolve(left, right);
    str op = self.op.string;

    if(className(&self.left) != "ComparativeExpression")
        return result.replace(fmt::format("{} {} {}", 
            left.toString(), op, right.toString()));

    ComparativeExpression* leftNode = static_cast<ComparativeExpression*>(&self.left);
    Transpiler::Line leftArg = leftNode->right.transpile(transpiler);

    return result.replace(fmt::format("{} && {} {} {}", 
        left.toString(), leftArg.toString(), op, right.toString())).cast("bool");
}
