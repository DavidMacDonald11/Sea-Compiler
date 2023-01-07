#ifndef FILE_STATEMENT_H
#define FILE_STATEMENT_H

#include "../node.h"

struct FileStatement : public Node {
    vector<Node*> statements;

    vector<Component*> nodes() const override;

    ~FileStatement();
    
    Transpiler::Line transpile() override;
    void makeTree();
};

#endif //FILE_STATEMENT_H
