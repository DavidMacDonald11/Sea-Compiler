#ifndef FILE_STATEMENT_H
#define FILE_STATEMENT_H

#include "../node.h"

struct FileStatement : public Node {
    vector<Node*> statements;

    vector<Component*> nodes() const override;

    ~FileStatement();
    
    void makeTree(Parser& parser);
    Publisher::Value* publish(Publisher& publisher) override;
    Transpiler::Line transpile(Transpiler& transpiler) override;
};

#endif //FILE_STATEMENT_H
