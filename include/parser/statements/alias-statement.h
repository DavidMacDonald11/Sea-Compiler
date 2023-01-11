#ifndef ALIAS_STATEMENT_H
#define ALIAS_STATEMENT_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct AliasStatement : public Node {
    Token* visibility;
    Node& type;
    Token& identifier;

    AliasStatement(Token* visibility, Node& type, Token& identifier);
    ~AliasStatement();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //ALIAS_STATEMENT_H
