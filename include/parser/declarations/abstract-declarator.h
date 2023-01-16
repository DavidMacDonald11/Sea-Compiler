#ifndef ABSTRACT_DECLARATOR_H
#define ABSTRACT_DECLARATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct AbstractDeclarator : public Node {
    Node* pointer;
    Node* declarator;

    AbstractDeclarator(Node* pointer, Node* declarator);
    ~AbstractDeclarator();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //ABSTRACT_DECLARATOR_H
