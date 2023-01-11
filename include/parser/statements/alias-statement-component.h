#ifndef ALIAS_STATEMENT_COMPONENT_H
#define ALIAS_STATEMENT_COMPONENT_H

#include "../node.h"

struct AliasStatementComponent : public Node {
    Token* visibility;
    Node& type;
    Token& identifier;

    AliasStatementComponent(Token* visibility, Node& type, Token& identifier);
    ~AliasStatementComponent();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //ALIAS_STATEMENT_COMPONENT_H
