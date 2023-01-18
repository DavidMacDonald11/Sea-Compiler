#include "parser/declarations/type-modifier-list.h"
#include "parser/declarations/align-specifier.h"
#include "parser/declarations/type-qualifier-list.h"

TypeModifierList::TypeModifierList(Node* qualifiers, Node* align)
: qualifiers(qualifiers), align(align) {}

TypeModifierList::~TypeModifierList() {
    delete qualifiers;
    delete align;
}

Nodes TypeModifierList::nodes() const {
    Nodes nodes;

    if(qualifiers) nodes.push_back(qualifiers);
    if(align) nodes.push_back(align);

    return nodes;
}

Node* TypeModifierList::construct(Parser &parser) {
    Node* qualifiers = TypeQualifierList::construct(parser);
    Node* align = nullptr;

    if(parser.next().has({"alignas"})) align = AlignSpecifier::construct(parser);
    if(not qualifiers and not align) return nullptr;

    return new TypeModifierList(qualifiers, align);
}
