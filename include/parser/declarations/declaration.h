#ifndef DECLARATION_H
#define DECLARATION_H

#include "../node.h"

struct Declaration : public Node {
    static Node* construct();
};

#endif //DECLARATION_H
