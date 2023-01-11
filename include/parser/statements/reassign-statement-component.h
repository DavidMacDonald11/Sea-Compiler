#ifndef REASSIGN_STATEMENT_COMPONENT_H
#define REASSIGN_STATEMENT_COMPONENT_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct ReassignStatementComponent : public BinaryOperation {
    ReassignStatementComponent(Node& left, Token& op, Node& right);
    static Node* construct(Node& left);
    Transpiler::Line transpile() override;
};

#endif //REASSIGN_STATEMENT_COMPONENT_H
