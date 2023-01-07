#include "parser/declarations/type-qualifier.h"
#include "parser/declarations/type-qualifier-list.h"

vector<Component*> TypeQualifierList::nodes() const {
    vector<Component*> nodes;
    for(Node* node : qualifiers) nodes.push_back(node);
    return nodes;
}

TypeQualifierList::TypeQualifierList(vector<Node*> qualifiers)
: qualifiers(qualifiers) {}

TypeQualifierList::~TypeQualifierList() {
    for(Node* node : qualifiers) delete node;
}

Node* TypeQualifierList::construct() {
    vector<Node*> nodes;

    while(parser->next().has(Token::TYPE_QUALIFIER_KEYWORDS)) 
        nodes.push_back(TypeQualifier::construct());

    return new TypeQualifierList(nodes);
}

Transpiler::Line TypeQualifierList::transpile() {
    Transpiler::Line result;

    for(Node* node : qualifiers) 
        result.add("", fmt::format("{} ", node->transpile().toString()));

    result.string.pop_back();
    return result;
}
