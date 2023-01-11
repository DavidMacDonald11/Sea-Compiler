#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include "../node.h"

struct InitializerList : public Node {
    using initializer = pair<Node*, Node*>;

    vector<initializer> initializers;

    InitializerList(vector<initializer> initializers);
    ~InitializerList();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //INITIALIZER_LIST_H
