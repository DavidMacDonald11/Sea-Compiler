#include "parser/declarations/declarator.h"
#include "parser/declarations/direct-declarator.h"
#include "parser/declarations/pointer.h"
#include "transpiler/transpiler.h"

Declarator::Declarator(Node* pointer, Node& declarator)
: pointer(pointer), declarator(declarator) {}

Declarator::~Declarator() {
    delete pointer;
    delete &declarator;
}

Nodes Declarator::nodes() const {
    return pointer? Nodes{pointer, &declarator} : Nodes{&declarator};
}

Node* Declarator::construct() {
    Node* pointer = Pointer::construct();
    return new Declarator(pointer, *DirectDeclarator::construct());
}

Transpiler::Line Declarator::transpile() {
    if(not pointer) return declarator.transpile();

    Transpiler::Line line = pointer->transpile();
    line.add("", declarator.transpile().toString());
    return line;
}
