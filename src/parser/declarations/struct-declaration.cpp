#include "parser/declarations/struct-declaration.h"
#include "parser/declarations/assert-declaration.h"
#include "parser/declarations/struct-declarator.h"
#include "parser/declarations/type-name.h"

StructDeclaration::StructDeclaration(Node& type, vector<Node*> declarators)
: type(type), declarators(declarators) {}

StructDeclaration::~StructDeclaration() {
    delete &type;
    for(Node* node : declarators) delete node;
}

Nodes StructDeclaration::nodes() const {
    Nodes nodes = {&type};
    nodes.reserve(declarators.size());

    for(Node* node : declarators) nodes.push_back(node);
    return nodes;
}

Node* StructDeclaration::construct(Parser& parser) {
    if(parser.nextOrAfterHas({"assert"})) {
        Node* node =  AssertDeclaration::construct(parser);
        parser.expectingHas(Token::LINE_ENDS);
        return node;
    }

    Node& type = *TypeName::construct(parser);
    vector<Node*> declarators;

    while(not parser.next().has(Token::LINE_ENDS)) {
        declarators.push_back(StructDeclarator::construct(parser));
        if(not parser.next().has({","})) break;

        parser.take();
        parser.skipNewlines();
    }

    parser.expectingHas(Token::LINE_ENDS);
    parser.skipNewlines();
    
    return new StructDeclaration(type, declarators);
}
