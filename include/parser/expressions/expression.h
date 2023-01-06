#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../node.h"

struct Expression : public Node {
    static Node* construct();
};

#endif //EXPRESSION_H
