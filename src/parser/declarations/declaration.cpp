#include "parser/declarations/declaration.h"
#include "parser/declarations/assert-declaration.h"
#include "parser/declarations/declaration-specifiers.h"
#include "parser/declarations/init-declarator.h"
#include "transpiler/transpiler.h"

Declaration::Declaration(Node* specifiers, vector<Node*> declarators)
: specifiers(specifiers), declarators(declarators) {}

Declaration::~Declaration() {
    delete specifiers;
    for(Node* node : declarators) delete node;
}

Nodes Declaration::nodes() const {
    Nodes nodes;

    if(specifiers) nodes.push_back(specifiers);
    for(Node* node : declarators) nodes.push_back(node);

    return nodes;
}

Node* Declaration::construct(Parser& parser) {
    if(parser.ahead(1).has({"assert"})) return AssertDeclaration::construct(parser);

    Node* specifiers = DeclarationSpecifiers::construct(parser);
    vector<Node*> nodes;

    while(true) {
        nodes.push_back(InitDeclarator::construct(parser));
        if(not parser.next().has({","})) break;
        
        parser.take();
        parser.skipNewlines();
    }

    return new Declaration(specifiers, nodes);
}

Transpiler::Line Declaration::transpile(Transpiler& transpiler) {
    Transpiler::Line line;
    if(specifiers) line = specifiers->transpile(transpiler);
    
    for(nat i = 0; i < declarators.size(); i++) {
        line.add("", declarators[i]->transpile(transpiler).toString());
        if(i != declarators.size() - 1) line.add("", ", ");
    }

    return line;
}
