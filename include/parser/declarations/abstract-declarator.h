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
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //ABSTRACT_DECLARATOR_H
