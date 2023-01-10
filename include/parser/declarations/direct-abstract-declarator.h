#ifndef DIRECT_ABSTRACT_DECLARATOR_H
#define DIRECT_ABSTRACT_DECLARATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct DirectAbstractDeclarator : public Node {
    Node& declarator;
    Node* array;

    DirectAbstractDeclarator(Node& declarator, Node* array);
    ~DirectAbstractDeclarator();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //DIRECT_ABSTRACT_DECLARATOR_H
