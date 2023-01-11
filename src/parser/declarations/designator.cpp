#include "parser/declarations/designator.h"
#include "parser/expressions/single-expression.h"
#include "transpiler/transpiler.h"

Designator::Designator(Token& token, Node* expression, Token* identifier) 
: token(token), expression(expression), identifier(identifier) {}

Designator::~Designator() {
    delete expression;
}

Nodes Designator::nodes() const {
    return expression? Nodes{&token, expression} : Nodes{&token, identifier};
}

Node* Designator::construct() {
    Token& token = parser->expectingHas({"#", "."});
    Node* expression = nullptr;
    Token* identifier = nullptr;

    if(token.has({"#"})) expression = SingleExpression::construct();
    else identifier = &parser->expectingOf({Token::IDENTIFIER});

    return new Designator(token, expression, identifier);
}

Transpiler::Line Designator::transpile() {
    if(token.has({"."})) return {"", "." + identifier->string};
    return expression->transpile().add("[", "]");
}
