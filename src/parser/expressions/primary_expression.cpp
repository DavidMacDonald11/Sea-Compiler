#include "parser/expressions/primary_expression.h"
#include "parser/node.h"
#include "util/component.h"
#include "util/fault.h"

Node* PrimaryExpression::construct() {
    if(parser->next().of({Token::NUM})) 
        return NumericConstant::construct();

    Token& failure = parser->take();

    throw Fault::fail(failure, fmt::format(
        "PrimaryExpression error; unexpected token {}", 
        failure.toString()));
}

NumericConstant::NumericConstant(Token& token) 
: PrimaryNode(token) {}

Node* NumericConstant::construct() {
    return new NumericConstant(parser->take());
}
