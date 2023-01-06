#ifndef STATEMENT_H
#define STATEMENT_H

#include "../node.h"
#include "util/component.h"

struct Statement : public Node {
    Node& statement;

    vector<Component*> nodes() const override;

    Statement(Node& statement);
    ~Statement();

    static Node* construct();
};

#endif //STATEMENT_H
