#ifndef STATEMENT_H
#define STATEMENT_H

#include "hidden-statement.h"

struct Statement : public HiddenStatement {
    Statement(Node& statement);
    static Node* construct();
};

#endif //STATEMENT_H
