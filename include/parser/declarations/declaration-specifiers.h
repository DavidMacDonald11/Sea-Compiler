#ifndef DECLARATION_SPECIFIERS_H
#define DECLARATION_SPECIFIERS_H

#include "../node.h"

struct DeclarationSpecifiers : public Node {
    Token* visibility;
    Token* storage;
    Node& list;

    DeclarationSpecifiers(Token* visibility, Token* storage, Node& list);
    ~DeclarationSpecifiers();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //DECLARATION_SPECIFIERS_H
