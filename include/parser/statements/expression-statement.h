#ifndef EXPRESSION_STATEMENT_H
#define EXPRESSION_STATEMENT_H

#include "hidden-statement.h"

struct ExpressionStatement : public HiddenStatement {
    ExpressionStatement(Node& expression);
    static Node* construct();
};

#endif //EXPRESSION_STATEMENT_H
