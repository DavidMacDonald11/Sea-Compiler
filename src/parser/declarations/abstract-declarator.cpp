#include "parser/declarations/abstract-declarator.h"
#include "parser/declarations/direct-declarator.h"
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

Node* AbstractDeclarator::construct(Parser& parser) {
    Node* pointer = Pointer::construct(parser);
    Node* declarator = DirectAbstractDeclarator::construct(parser);

    if(not declarator and parser.context.allowDirectAbstractDeclarator)
        declarator = DirectDeclarator::construct(parser);

    if(not pointer and not declarator) return nullptr;
    return new AbstractDeclarator(pointer, declarator);
}

Transpiler::Line AbstractDeclarator::transpile(Transpiler& transpiler) {
    if(not pointer) return declarator->transpile(transpiler);

    Transpiler::Line line = pointer->transpile(transpiler);
    if(declarator) line.add("", declarator->transpile(transpiler).toString());
    return line;
}
