#ifndef ASSERT_DECLARATION_H
#define ASSERT_DECLARATION_H

#include "../node.h"

struct AssertDeclaration : public Node {
    Token* token;
    Node& assertion;
    Node& message;

    AssertDeclaration(Token* token, Node& assertion, Node& message);
    ~AssertDeclaration();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //ASSERT_DECLARATION_H
