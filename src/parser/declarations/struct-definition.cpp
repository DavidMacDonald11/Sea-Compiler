#include "parser/declarations/struct-definition.h"
#include "parser/declarations/struct-declaration.h"

StructDefinition::StructDefinition(Token* visibility, Token& keyword, Token& name, vector<Node*> list)
: visibility(visibility), keyword(keyword), name(name), list(list) {}

StructDefinition::~StructDefinition() {
    for(Node* node : list) delete node;
}

Nodes StructDefinition::nodes() const {
    Nodes nodes;
    nodes.reserve(list.size());

    for(Node* node : list) nodes.push_back(node);
    return nodes;
}

Node* StructDefinition::construct(Parser& parser) {
    if(not parser.nextOrAfterHas({"struct", "union"})) return nullptr;
    
    Token* visibility = nullptr;
    if(parser.next().has(Token::VISIBILITY_KEYWORDS)) visibility = &parser.take();

    Token& keyword = parser.expectingHas({"struct", "union"});
    Token& name = parser.expectingOf({Token::IDENTIFIER});

    parser.expectingHas({"{"});
    parser.skipNewlines();
    
    vector<Node*> list;

    while(not parser.next().has({"}"})) {
        list.push_back(StructDeclaration::construct(parser));
    }

    parser.expectingHas({"}"});
    return new StructDefinition(visibility, keyword, name, list);
}

Publisher::Value* StructDefinition::publish(Publisher& publisher) {
    auto value = new Publisher::Declaration(name.string);
    publisher.table.insert(pair<str, Publisher::Value*>(name.string, value));
    
    return value;
}
