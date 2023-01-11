#include "parser/declarations/declaration-specifiers.h"
#include "parser/declarations/specifier-qualifier-list.h"
#include "transpiler/transpiler.h"
#include <string>

DeclarationSpecifiers::DeclarationSpecifiers(Token* visibility, Token* storage, Node& list)
: visibility(visibility), storage(storage), list(list) {}

DeclarationSpecifiers::~DeclarationSpecifiers() {
    delete &list;
}

Nodes DeclarationSpecifiers::nodes() const {
    Nodes nodes;

    if(visibility) nodes.push_back(visibility);
    if(storage) nodes.push_back(storage);
    nodes.push_back(&list);

    return nodes;
}

Node* DeclarationSpecifiers::construct() {
    Token* visibility = nullptr;
    Token* storage = nullptr;

    if(parser->next().has(Token::VISIBILITY_KEYWORDS)) visibility = &parser->take();
    if(parser->next().has(Token::STORAGE_KEYWORDS)) storage = &parser->take();

    return new DeclarationSpecifiers(visibility, storage, *SpecifierQualifierList::construct());
}

Transpiler::Line DeclarationSpecifiers::transpile() {
    Transpiler::Line line;

    if(storage and not storage->has({"future"})) line.replace(storage->string);
    line.add("", " " + list.transpile().toString());

    return line.add("", " ");
}
