#include "parser/expressions/expression.h"
#include "parser/expressions/primary-expression.h"
#include "parser/node.h"
#include "fault.h"

Node* PrimaryExpression::construct() {
    Token& next = parser->next();

    if(next.of({Token::NUM})) return NumericConstant::construct();
    if(next.of({Token::IDENTIFIER})) return Identifier::construct();
    if(next.has({"("})) return ParentheseseExpression::construct();
    if(next.has(Token::PRIMARY_KEYWORDS)) return PrimaryKeyword::construct();

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


Node* ParentheseseExpression::construct() {
    parser->expectingHas({"("});
    Node* node = Expression::construct();
    parser->expectingHas({")"});
    return node;
}


PrimaryKeyword::PrimaryKeyword(Token& token)
: PrimaryNode(token) {}

Node* PrimaryKeyword::construct() {
    return new PrimaryKeyword(parser->take());
}

// TODO character, string
