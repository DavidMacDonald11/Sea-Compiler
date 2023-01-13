#include "parser/declarations/enum-definition.h"
#include "parser/declarations/enumerator.h"

EnumDefinition::EnumDefinition(Token* visibility, Token& identifier, vector<Node*> enumerators)
: visibility(visibility), identifier(identifier), enumerators(enumerators) {}

EnumDefinition::~EnumDefinition() {
    for(Node* node : enumerators) delete node;
}

Nodes EnumDefinition::nodes() const {
    Nodes nodes;
    nodes.reserve(enumerators.size());

    if(visibility) nodes.push_back(visibility);
    nodes.push_back(&identifier);
    for(Node* node : enumerators) nodes.push_back(node);

    return nodes;
}

Node* EnumDefinition::construct() {
    if(not parser->nextOrAfterHas({"enum"})) return nullptr;

    Token* visibility = nullptr;
    if(parser->nextOrAfterHas(Token::VISIBILITY_KEYWORDS)) visibility = &parser->take();

    parser->expectingHas({"enum"});
    Token& identifier = parser->expectingOf({Token::IDENTIFIER});

    parser->expectingHas({"{"});
    parser->skipNewlines();
    
    vector<Node*> enumerators;

    while(not parser->next().has({"}"})) {
        enumerators.push_back(Enumerator::construct());
        if(parser->next().has({","})) parser->take();
    }

    parser->expectingHas({"}"});
    return new EnumDefinition(visibility, identifier, enumerators);
}
