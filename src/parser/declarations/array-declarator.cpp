#include "parser/declarations/array-declarator.h"
#include "parser/declarations/type-qualifier-list.h"
#include "parser/expressions/single-expression.h"
#include "transpiler/transpiler.h"

ArrayDeclarator::ArrayDeclarator(Node* qualifiers, Token* token, Node* expression)
: qualifiers(qualifiers), token(token), expression(expression) {}

ArrayDeclarator::~ArrayDeclarator() {
    delete qualifiers;
    delete expression;
}

Nodes ArrayDeclarator::nodes() const {
    Nodes nodes;

    if(qualifiers) nodes.push_back(qualifiers);
    if(token) nodes.push_back(token);
    if(expression) nodes.push_back(expression);

    return nodes;
}

Node* ArrayDeclarator::construct() {
    parser->expectingHas({"["});
    Node* qualifiers = TypeQualifierList::construct();
    Token* token = nullptr;
    Node* expression = nullptr;
    
    if(parser->next().has({"*"})) token = &parser->take();
    else if(parser->next().has({"<="})) {
        token = &parser->take();
        expression = SingleExpression::construct();
    } else if(not parser->next().has({"]"})) {
        expression = SingleExpression::construct();
    }

    parser->expectingHas({"]"});
    return new ArrayDeclarator(qualifiers, token, expression);
}

Transpiler::Line ArrayDeclarator::transpile() {
    Transpiler::Line line = {"", "["};

    if(qualifiers) line.add("", qualifiers->transpile().toString());
    if(token) line.add("", (token->has({"*"}))? " * " : " static ");
    if(expression) line.add("", expression->transpile().toString());

    return line.add("", "]");
}
