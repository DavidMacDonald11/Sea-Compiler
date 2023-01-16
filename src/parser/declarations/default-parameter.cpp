#include "parser/declarations/default-parameter.h"
#include "parser/declarations/parameter.h"
#include "parser/expressions/single-expression.h"

DefaultParameter::DefaultParameter(Node& parameter, Node& expression)
: parameter(parameter), expression(expression) {}

DefaultParameter::~DefaultParameter() {
    delete &parameter;
    delete &expression;
}

Nodes DefaultParameter::nodes() const {
    return {&parameter, &expression};
}

Node* DefaultParameter::construct(Parser& parser) {
    Node& parameter = *Parameter::construct(parser);
    if(not parser.next().has({"="})) return &parameter;

    parser.take();
    return new DefaultParameter(parameter, *SingleExpression::construct(parser));
}
