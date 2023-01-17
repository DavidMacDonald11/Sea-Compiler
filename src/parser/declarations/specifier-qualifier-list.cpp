#include "parser/declarations/specifier-qualifier-list.h"
#include "parser/declarations/align-specifier.h"
#include "parser/declarations/type-qualifier-list.h"
#include "parser/declarations/type-specifier.h"
#include "publisher/publisher.h"

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

Node* SpecifierQualifierList::construct(Parser& parser) {
    Node* qualifiers = (parser.next().has(Token::TYPE_QUALIFIER_KEYWORDS))? 
        TypeQualifierList::construct(parser) : nullptr;

    Node* align = (parser.next().has({"alignas"}))? 
        AlignSpecifier::construct(parser) : nullptr;

    Node* type = TypeSpecifier::construct(parser);
    return new SpecifierQualifierList(qualifiers, align, *type);
}

Publisher::Value* SpecifierQualifierList::publish(Publisher &publisher) {
    Publisher::Type* value = static_cast<Publisher::Type*>(type.publish(publisher));
    Publisher::Type* list = static_cast<Publisher::Type*>(qualifiers->publish(publisher));

    value->qualifiers = list->qualifiers;
    delete list;

    return value;
}

Transpiler::Line SpecifierQualifierList::transpile(Transpiler& transpiler) {
    Transpiler::Line line = type.transpile(transpiler);

    if(align) line.add(fmt::format("{} ", align->transpile(transpiler).toString()));
    if(qualifiers) line.add(fmt::format("{} ", qualifiers->transpile(transpiler).toString()));

    return line;
}
