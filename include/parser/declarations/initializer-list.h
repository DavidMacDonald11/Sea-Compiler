#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include "../node.h"

struct InitializerList : public Node {
    using initializer = pair<Node*, Node*>;

    vector<initializer> initializers;

    InitializerList(vector<initializer> initializers);
    ~InitializerList();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //INITIALIZER_LIST_H
