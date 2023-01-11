#include "parser/declarations/init-declarator.h"
#include "parser/declarations/declarator.h"
#include "parser/expressions/single-expression.h"
#include "transpiler/transpiler.h"

InitDeclarator::InitDeclarator(Node& declarator, Node* expression)
: declarator(declarator), expression(expression) {}

InitDeclarator::~InitDeclarator() {
    delete &declarator;
    delete expression;
}

Nodes InitDeclarator::nodes() const {
    return expression? Nodes{&declarator, expression} : Nodes{&declarator};
}

Node* InitDeclarator::construct() {
    Node* declarator = Declarator::construct();
    Node* identifier = nullptr;
    
    if(parser->next().has({"="})) {
        parser->take();
        identifier = SingleExpression::construct();
    }

    return new InitDeclarator(*declarator, identifier);
}

Transpiler::Line InitDeclarator::transpile() {
    Transpiler::Line line = declarator.transpile();
    if(expression) line.add("", " = " + expression->transpile().toString());
    return line;
}
