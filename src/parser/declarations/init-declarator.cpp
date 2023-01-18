#include "parser/declarations/init-declarator.h"
#include "parser/expressions/single-expression.h"
#include "transpiler/transpiler.h"

InitDeclarator::InitDeclarator(Token& identifier, Node* expression)
: identifier(identifier), expression(expression) {}

InitDeclarator::~InitDeclarator() {
    delete expression;
}

Nodes InitDeclarator::nodes() const {
    return expression? Nodes{&identifier, expression} : Nodes{&identifier};
}

Node* InitDeclarator::construct(Parser& parser) {
    Token& identifier = parser.expectingOf({Token::IDENTIFIER});
    Node* expression = nullptr;
    
    if(parser.next().has({"="})) {
        parser.take();
        expression = SingleExpression::construct(parser);
    }

    return new InitDeclarator(identifier, expression);
}
