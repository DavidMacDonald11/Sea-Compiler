#ifndef DECLARATION_SPECIFIERS_H
#define DECLARATION_SPECIFIERS_H

#include "../node.h"

struct DeclarationSpecifiers : public Node {
    Token* visibility;
    Token* storage;
    Node& type;

    DeclarationSpecifiers(Token* visibility, Token* storage, Node& type);
    ~DeclarationSpecifiers();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //DECLARATION_SPECIFIERS_H
