#ifndef ENUMERATOR_H
#define ENUMERATOR_H

#include "../node.h"

struct Enumerator : public Node {
    Token& identifier;
    Node* expression;

    Enumerator(Token& identifier, Node* expression);
    ~Enumerator();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //ENUMERATOR_H
