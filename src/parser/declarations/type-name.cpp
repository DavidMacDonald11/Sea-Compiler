#include "parser/declarations/type-name.h"
#include "parser/declarations/type-modifier-list.h"
#include "parser/declarations/type-specifier.h"
#include "parser/declarations/pointers.h"
#include "transpiler/transpiler.h"

TypeName::TypeName(Node* list, Node& type, Node* pointers)
: list(list), type(type), pointers(pointers) {}

TypeName::~TypeName() {
    delete list;
    delete &type;
    delete pointers;
}

Nodes TypeName::nodes() const {
    Nodes nodes;

    if(list) nodes.push_back(list);
    nodes.push_back(&type);
    if(pointers) nodes.push_back(pointers);

    return nodes;
}

Node* TypeName::construct(Parser& parser) {
    Node* list = TypeModifierList::construct(parser);
    Node& type = *TypeSpecifier::construct(parser);
    return new TypeName(list, type, Pointers::construct(parser));
}
