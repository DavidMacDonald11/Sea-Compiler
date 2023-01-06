#include "parser/expressions/primary_expression.h"
#include "parser/node.h"
#include "util/component.h"
#include "util/fault.h"

Node* PrimaryExpression::construct() {
    if(parser->next().of({Token::NUM})) 
        return NumericConstant::construct();

    if(parser->next().of({Token::IDENTIFIER}))
        return Identifier::construct();

    if(parser->next().has(Token::PRIMARY_KEYWORDS))
        return PrimaryKeyword::construct();

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


Identifier::Identifier(Token& token)
: PrimaryNode(token) {}

Node* Identifier::construct() {
    return new Identifier(parser->take());
}


PrimaryKeyword::PrimaryKeyword(Token& token)
: PrimaryNode(token) {}

Node* PrimaryKeyword::construct() {
    return new PrimaryKeyword(parser->take());
}
