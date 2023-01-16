#include "parser/declarations/initializer-list.h"
#include "parser/declarations/designation.h"
#include "parser/expressions/single-expression.h"
#include "transpiler/transpiler.h"

InitializerList::InitializerList(vector<initializer> initializers) 
: initializers(initializers) {}

InitializerList::~InitializerList() {
    for(initializer pair : initializers) {
        delete pair.first;
        delete pair.second;
    }
}

Nodes InitializerList::nodes() const {
    Nodes nodes;

    for(initializer pair : initializers) {
        if(pair.first) nodes.push_back(pair.first);
        nodes.push_back(pair.second);
    }

    return nodes;
}

Node* InitializerList::construct(Parser& parser) {
    vector<initializer> nodes;

    while(true) {
        Node* desig = Designation::construct(parser);
        Node* init = SingleExpression::construct(parser);

        nodes.push_back({desig, init});
        if(not parser.next().has({","})) break;

        parser.take();
        parser.skipNewlines();
        
        if(parser.next().has({"]"})) break;
    }

    return new InitializerList(nodes);
}

Transpiler::Line InitializerList::transpile(Transpiler& transpiler) {
    Transpiler::Line line;

    for(initializer pair : initializers) {
        if(pair.first) line.add("", pair.first->transpile(transpiler).toString());
        line.add("", pair.second->transpile(transpiler).toString());
        line.add("", ", ");
    }

    return line;
}
