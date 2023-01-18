#include "parser/declarations/declaration-specifiers.h"
#include "parser/declarations/type-name.h"
#include "transpiler/transpiler.h"
#include <string>

DeclarationSpecifiers::DeclarationSpecifiers(Token* visibility, Token* storage, Node& type)
: visibility(visibility), storage(storage), type(type) {}

DeclarationSpecifiers::~DeclarationSpecifiers() {
    delete &type;
}

Nodes DeclarationSpecifiers::nodes() const {
    Nodes nodes;

    if(visibility) nodes.push_back(visibility);
    if(storage) nodes.push_back(storage);
    nodes.push_back(&type);

    return nodes;
}

Node* DeclarationSpecifiers::construct(Parser& parser) {
    Token* visibility = nullptr;
    Token* storage = nullptr;

    if(parser.next().has(Token::VISIBILITY_KEYWORDS)) visibility = &parser.take();
    if(parser.next().has(Token::STORAGE_KEYWORDS)) storage = &parser.take();

    return new DeclarationSpecifiers(visibility, storage, *TypeName::construct(parser));
}
