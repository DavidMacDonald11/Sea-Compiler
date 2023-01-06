#include "parser/expressions/expression.h"
#include "parser/expressions/primary-expression.h"
#include "parser/node.h"
#include "fault.h"
#include "transpiler/transpiler.h"

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

Transpiler::Expression NumericConstant::transpile() {
    str type = in('.', token.string)? "float" : "int";
    return {type, token.string};
}


Identifier::Identifier(Token& token)
: PrimaryNode(token) {}

Node* Identifier::construct() {
    return new Identifier(parser->take());
}

Transpiler::Expression Identifier::transpile() {
    str type = "IDENTIFIER";
    return {type, token.string};
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

Transpiler::Expression PrimaryKeyword::transpile() {
    str type = (token.string == "null")? "byte" : "bool";
    str string = (token.string == "true")? "1" : "0";
    return {type, string};
}

// TODO character, string
