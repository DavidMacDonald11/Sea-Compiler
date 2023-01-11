#ifndef DECLARATOR_H
#define DECLARATOR_H

#include "../node.h"

struct Declarator : public Node {
    Node* pointer;
    Node& declarator;

    Declarator(Node* pointer, Node& declarator);
    ~Declarator();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //DECLARATOR_H
