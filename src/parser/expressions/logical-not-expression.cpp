#include "parser/expressions/comparative-expression.h"
#include "parser/expressions/logical-not-expression.h"

vector<Component*> LogicalNotExpression::nodes() const { return {&expression}; }

LogicalNotExpression::LogicalNotExpression(Node& expression)
: expression(expression) {}

LogicalNotExpression::~LogicalNotExpression() {
    delete &expression;
}

Node* LogicalNotExpression::construct(Parser& parser) {
    if(parser.next().has({"not"})) {
        parser.take();
        return new LogicalNotExpression(*LogicalNotExpression::construct(parser));
    }
    
    return ComparativeExpression::construct(parser);
}

Transpiler::Line LogicalNotExpression::transpile(Transpiler& transpiler) {
    return expression.transpile(transpiler).add("!(", ")").cast("bool");
}
