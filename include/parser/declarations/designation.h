#ifndef DESIGNATION_H
#define DESIGNATION_H

#include "../node.h"
#include "transpiler/transpiler.h"

struct Designation : public Node {
    vector<Node*> designators;

    Designation(vector<Node*> designators);
    ~Designation();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //DESIGNATION_H
