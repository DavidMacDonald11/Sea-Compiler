#include "parser/declarations/abstract-declarator.h"
#include "parser/declarations/direct-abstract-declarator.h"
#include "parser/declarations/pointer.h"
#include "transpiler/transpiler.h"

AbstractDeclarator::AbstractDeclarator(Node* pointer, Node* declarator)
: pointer(pointer), declarator(declarator) {}

AbstractDeclarator::~AbstractDeclarator() {
    delete pointer;
    delete declarator;
}

Nodes AbstractDeclarator::nodes() const {
    if(not pointer) return {declarator};

    Nodes nodes = {pointer};
    if(declarator) nodes.push_back(declarator);
    return nodes;
}

Node* AbstractDeclarator::construct() {
    Node* pointer = Pointer::contruct();
    Node* declarator = DirectAbstractDeclarator::construct();
    
    if(not pointer and not declarator) return nullptr;
    return new AbstractDeclarator(pointer, declarator);
}

Transpiler::Line AbstractDeclarator::transpile() {
    if(not pointer) return declarator->transpile();

    Transpiler::Line line = pointer->transpile();
    if(declarator) line.add("", declarator->transpile().toString());
    return line;
}
