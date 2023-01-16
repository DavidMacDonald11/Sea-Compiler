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

Node* Declarator::construct(Parser& parser) {
    Node* pointer = Pointer::construct(parser);
    return new Declarator(pointer, *DirectDeclarator::construct(parser));
}

Transpiler::Line Declarator::transpile(Transpiler& transpiler) {
    if(not pointer) return declarator.transpile(transpiler);

    Transpiler::Line line = pointer->transpile(transpiler);
    line.add("", declarator.transpile(transpiler).toString());
    return line;
}
