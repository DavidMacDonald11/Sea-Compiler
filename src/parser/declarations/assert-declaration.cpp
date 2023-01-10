#include "parser/declarations/assert-declaration.h"
#include "parser/expressions/single-expression.h"

AssertDeclaration::AssertDeclaration(Token* token, Node& assertion, Node& message) 
: token(token), assertion(assertion), message(message) {}

AssertDeclaration::~AssertDeclaration() {
    delete &assertion;
    delete &message;
}

Nodes AssertDeclaration::nodes() const {
    return token? Nodes{token, &assertion, &message} 
        : Nodes{&assertion, &message};
}

Node* AssertDeclaration::construct() {
    Token* token = (parser->next().has({"static"}))? &parser->take() : nullptr;
    parser->expectingHas({"assert"});

    Node* assertion = SingleExpression::construct();
    parser->expectingHas({"else"});

    Node* message = SingleExpression::construct();
    return new AssertDeclaration(token, *assertion, *message);
}
