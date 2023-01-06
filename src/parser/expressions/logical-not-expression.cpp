#include "parser/expressions/comparative-expression.h"
#include "parser/expressions/logical-not-expression.h"

vector<Component*> LogicalNotExpression::nodes() const { return {&expression}; }

LogicalNotExpression::LogicalNotExpression(Node& expression)
: expression(expression) {}

LogicalNotExpression::~LogicalNotExpression() {
    delete &expression;
}

Node* LogicalNotExpression::construct() {
    if(parser->next().has({"not"})) {
        parser->take();
        return new LogicalNotExpression(*LogicalNotExpression::construct());
    }
    
    return ComparativeExpression::construct();
}
