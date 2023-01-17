#ifndef ARRAY_DECLARATOR_H
#define ARRAY_DECLARATOR_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct ArrayDeclarator : public Node {
    Node* qualifiers;
    Token* token;
    Node* expression;

    ArrayDeclarator(Node* qualifiers, Token* token, Node* expression);
    ~ArrayDeclarator();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Publisher::Value* publish(Publisher& publisher) override;
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //ARRAY_DECLARATOR_H
