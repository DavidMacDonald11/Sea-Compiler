#ifndef FILE_STATEMENT_H
#define FILE_STATEMENT_H

#include "../node.h"

struct FileStatement : public Node {
    vector<Component*> statements;

    vector<Component*> nodes() const override;

    ~FileStatement();
    
    void makeTree();
};

#endif //FILE_STATEMENT_H
