#include "parser/declarations/specifier-qualifier-list.h"
#include "parser/declarations/align-specifier.h"
#include "parser/declarations/type-qualifier-list.h"
#include "parser/declarations/type-specifier.h"

vector<Component*> SpecifierQualifierList::nodes() const {
    vector<Component*> nodes;

    if(qualifiers) nodes.push_back(qualifiers);
    if(align) nodes.push_back(align);
    nodes.push_back(&type);

    return nodes;
}

SpecifierQualifierList::SpecifierQualifierList(Node* qualifiers, Node* align, Node& type)
: qualifiers(qualifiers), align(align), type(type) {}

SpecifierQualifierList::~SpecifierQualifierList() {
    delete qualifiers;
    delete align;
    delete &type;
}

Node* SpecifierQualifierList::construct() {
    Node* qualifiers = (parser->next().has(Token::TYPE_QUALIFIER_KEYWORDS))? 
        TypeQualifierList::construct() : nullptr;

    Node* align = (parser->next().has({"align"}))? 
        AlignSpecifier::construct() : nullptr;

    Node* type = TypeSpecifier::construct();
    return new SpecifierQualifierList(qualifiers, align, *type);
}

Transpiler::Line SpecifierQualifierList::transpile() {
    Transpiler::Line line = type.transpile();

    if(align) line.add(fmt::format("{} ", align->transpile().toString()));
    if(qualifiers) line.add(fmt::format("{} ", qualifiers->transpile().toString()));

    return line;
}
