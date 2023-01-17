#include "parser/declarations/array-declarator.h"
#include "parser/declarations/type-qualifier-list.h"
#include "parser/expressions/single-expression.h"
#include "publisher/publisher.h"
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

Node* ArrayDeclarator::construct(Parser& parser) {
    parser.expectingHas({"["});
    Node* qualifiers = TypeQualifierList::construct(parser);
    Token* token = nullptr;
    Node* expression = nullptr;
    
    if(parser.next().has({"*"})) token = &parser.take();
    else if(parser.next().has({"<="})) {
        token = &parser.take();
        expression = SingleExpression::construct(parser);
    } else if(not parser.next().has({"]"})) {
        expression = SingleExpression::construct(parser);
    }

    parser.expectingHas({"]"});
    return new ArrayDeclarator(qualifiers, token, expression);
}

Publisher::Value* ArrayDeclarator::publish(Publisher &publisher) {
    if(not qualifiers) return new Publisher::Declarator();
    
    Publisher::Type* type = static_cast<Publisher::Type*>(qualifiers->publish(publisher));
    Publisher::Declarator* value = new Publisher::Declarator({.qualifiers = type->qualifiers});
    delete type;

    return value;
}

Transpiler::Line ArrayDeclarator::transpile(Transpiler& transpiler) {
    Transpiler::Line line = {"", "["};

    if(qualifiers) line.add("", qualifiers->transpile(transpiler).toString());
    if(token) line.add("", (token->has({"*"}))? " * " : " static ");
    if(expression) line.add("", expression->transpile(transpiler).toString());

    return line.add("", "]");
}
