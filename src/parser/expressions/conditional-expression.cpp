#include "parser/expressions/conditional-expression.h"
#include "parser/expressions/expression.h"
#include "parser/expressions/logical-or-expression.h"

vector<Component*> ConditionalExpression::nodes() const {
    return {&left, &condition, &right};
}

ConditionalExpression::ConditionalExpression(Node& left, Node& condition, Node& right)
: left(left), condition(condition), right(right) {}

ConditionalExpression::~ConditionalExpression() {
    delete &left;
    delete &condition;
    delete &right;
}

Node* ConditionalExpression::construct() {
    Node* node = LogicalOrExpression::construct();
    if(not parser->next().has({"if"})) return node;

    parser->take();
    Node& condition = *Expression::construct();
    parser->expectingHas({"else"});
    
    return new ConditionalExpression(*node, condition, *ConditionalExpression::construct());
}
