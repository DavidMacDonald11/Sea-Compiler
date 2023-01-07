#ifndef SINGLE_EXPRESSION_H
#define SINGLE_EXPRESSION_H

#include "../node.h"

struct SingleExpression : public Node {
    static Node* construct();
};

#endif //SINGLE_EXPRESSION_H
