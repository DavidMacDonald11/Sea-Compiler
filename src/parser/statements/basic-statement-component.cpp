#include "parser/statements/basic-statement-component.h"
#include "parser/expressions/expression.h"
#include "parser/node.h"

Node* BasicStatementComponent::construct() {
    Node* node = PassStatementComponent::construct();
    node = node? node : JumpStatementComponent::construct();
    node = node? node : ReturnStatementComponent::construct();

    return node;
}


PassStatementComponent::PassStatementComponent(Token& token)
: PrimaryNode(token) {}

Node* PassStatementComponent::construct() {
    if(not parser->next().has({"pass"})) return nullptr;
    return new PassStatementComponent(parser->take());
}


JumpStatementComponent::JumpStatementComponent(Token& token, Token* label)
: token(token), label(label) {}

Nodes JumpStatementComponent::nodes() const { 
    if(label) return {&token, label}; 
    return {&token};
}

Node* JumpStatementComponent::construct() {
    if(not parser->next().has({"continue", "break"})) return nullptr;

    Token& token = parser->take();
    Token* label = (parser->next().of({Token::IDENTIFIER}))? &parser->take() : nullptr;

    return new JumpStatementComponent(token, label);
}


ReturnStatementComponent::ReturnStatementComponent(Token& token, Node* expression)
: token(token), expression(expression) {}

ReturnStatementComponent::~ReturnStatementComponent() { delete expression; }

Nodes ReturnStatementComponent::nodes() const {
    if(expression) return {&token, expression};
    return {&token};
}

Node* ReturnStatementComponent::construct() {
    if(not parser->next().has({"return"})) return nullptr;

    Token& token = parser->take();
    Node* expression = nullptr;

    if(not parser->next().has(Token::LINE_ENDS)) expression = Expression::construct();
    return new ReturnStatementComponent(token, expression);
}
