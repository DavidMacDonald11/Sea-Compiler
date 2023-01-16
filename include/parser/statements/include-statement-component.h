#ifndef INCLUDE_STATEMENT_COMPONENT_H
#define INCLUDE_STATEMENT_COMPONENT_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct IncludeStatementComponent : public Node {
    Token* lib;
    Token& token;

    IncludeStatementComponent(Token* lib, Token& token);

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //INCLUDE_STATEMENT_COMPONENT_H
