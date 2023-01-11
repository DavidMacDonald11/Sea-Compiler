#include "parser/declarations/designation.h"
#include "parser/declarations/designator.h"
#include "transpiler/transpiler.h"

Designation::Designation(vector<Node*> designators) 
: designators(designators) {}

Designation::~Designation() {
    for(Node* node : designators) delete node;
}

Nodes Designation::nodes() const {
    Nodes nodes;
    for(Node* node : designators) nodes.push_back(node);
    return nodes;
}

Node* Designation::construct() {
    vector<Node*> nodes;

    while(parser->next().has({"#", "."})) {
        nodes.push_back(Designator::construct());
    }

    if(nodes.size() == 0) return nullptr;

    parser->expectingHas({"="});
    return new Designation(nodes);
}

Transpiler::Line Designation::transpile() {
    Transpiler::Line line;
    for(Node* node : designators) line.add("", node->transpile().toString() + " ");
    return line.add("", "= ");
}
