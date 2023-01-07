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

Node* Expression::construct() {
    Node* node = SingleExpression::construct();

    if(parser->next().has({","})) {
        parser->take();
        parser->skipNewlines();
        node = new Expression(*node, *Expression::construct());
    }

    return node;
}

Transpiler::Line Expression::transpile() {
    Transpiler::Line line = right.transpile();
    return line.add(fmt::format("{}, ", left.transpile().toString()));
}
