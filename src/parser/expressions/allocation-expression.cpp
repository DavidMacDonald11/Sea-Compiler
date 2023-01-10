#include "parser/expressions/allocation-expression.h"
#include "parser/expressions/bitwise-or-expression.h"

Node* AllocationExpression::construct() {
    Token& next = parser->next();

    if(next.has({"alloc"})) return AllocExpression::construct();
    if(next.has({"realloc"})) return ReallocExpression::construct();
    if(next.has({"unalloc"})) return UnallocExpression::construct();

    return BitwiseOrExpression::construct();
}


AllocExpression::AllocExpression(Node& expression, Token* token)
: expression(expression), token(token) {}

AllocExpression::~AllocExpression() {
    delete &expression;
}

Nodes AllocExpression::nodes() const {
    Nodes nodes = {&expression};
    if(token) nodes.push_back(token);
    return nodes;
}

Node* AllocExpression::construct() {
    parser->take();

    Node* node = BitwiseOrExpression::construct();
    Token* token = (parser->next().has({"null"}))? &parser->take() : nullptr;

    return new AllocExpression(*node, token);
}


ReallocExpression::ReallocExpression(Node& expression, Node& size)
: expression(expression), size(size) {}

ReallocExpression::~ReallocExpression() {
    delete &expression,
    delete &size;
}

Nodes ReallocExpression::nodes() const {
    return {&expression, &size};
}

Node* ReallocExpression::construct() {
    parser->take();

    Node* expression = AllocationExpression::construct();
    parser->expectingHas({"with"});
    Node* size = BitwiseOrExpression::construct();

    return new ReallocExpression(*expression, *size);
}


UnallocExpression::UnallocExpression(Node& expression) 
: expression(expression) {}

UnallocExpression::~UnallocExpression() {
    delete &expression;
}

Nodes UnallocExpression::nodes() const {
    return {&expression};
}

Node* UnallocExpression::construct() {
    parser->take();
    Node* node = AllocationExpression::construct();
    return new UnallocExpression(*node);
}
