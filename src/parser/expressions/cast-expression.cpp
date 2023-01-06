#include "parser/expressions/bitwise-or-expression.h"
#include "parser/expressions/cast-expression.h"
#include "util/component.h"

vector<Component*> CastExpression::nodes() const {
    return {&expression, &type};
}

CastExpression::CastExpression(Node& expression, Node& type)
: expression(expression), type(type) {}

CastExpression::~CastExpression() {
    delete &expression;
    delete &type;
}

Node* CastExpression::construct() {
    Node* node = BitwiseOrExpression::construct();

    // while(parser->next().has({"as"})) {
    //     Node* type = 
    //     node = new CastExpression(*node, *type);
    // }

    return node;
}

// TODO implement type-name

