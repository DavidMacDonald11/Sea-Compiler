#ifndef ARRAY_STYLE_POINTER_H
#define ARRAY_STYLE_POINTER_H

#include "../node.h"

struct ArrayStylePointer : public Node {
    Node* qualifiers;
    Token* token;
    Node* expression;

    ArrayStylePointer(Node* qualifiers, Token* token, Node* expression);
    ~ArrayStylePointer();

    Nodes nodes() const override;
    static Node* construct(Parser& parser, Node* qualifiers);
};

#endif //ARRAY_STYLE_POINTER_H
