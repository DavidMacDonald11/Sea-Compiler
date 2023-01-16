#include "parser/expressions/expression.h"
#include "parser/expressions/single-expression.h"

vector<Component*> Expression::nodes() const {
    return {&left, &right};
}

Expression::Expression(Node& left, Node& right) 
: left(left), right(right) {}

Expression::~Expression() {
    delete &left;
    delete &right;
}

Node* Expression::construct(Parser& parser) {
    Node* node = SingleExpression::construct(parser);

    if(parser.next().has({","})) {
        parser.take();
        parser.skipNewlines();
        node = new Expression(*node, *Expression::construct(parser));
    }

    return node;
}

Transpiler::Line Expression::transpile(Transpiler& transpiler) {
    Transpiler::Line line = right.transpile(transpiler);
    return line.add(fmt::format("{}, ", left.transpile(transpiler).toString()));
}
