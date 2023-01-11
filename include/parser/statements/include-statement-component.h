#ifndef INCLUDE_STATEMENT_COMPONENT_H
#define INCLUDE_STATEMENT_COMPONENT_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct IncludeStatementComponent : public PrimaryNode {
    IncludeStatementComponent(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //INCLUDE_STATEMENT_COMPONENT_H
