#ifndef FILE_STATEMENT_H
#define FILE_STATEMENT_H

#include "../node.h"

struct FileStatement : public Node {
    vector<Node*> statements;

    vector<Component*> nodes() const override;

    ~FileStatement();
    
    Transpiler::Line transpile(Transpiler& transpiler) override;
    void makeTree(Parser& parser);
};

#endif //FILE_STATEMENT_H
