#include "parser/declarations/direct-declarator.h"
#include "parser/declarations/array-declarator.h"
#include "parser/declarations/declarator.h"
#include "transpiler/transpiler.h"

DirectDeclarator::DirectDeclarator(Token* identifier, Node* declarator, Node* array)
: identifier(identifier), declarator(declarator), array(array) {}

DirectDeclarator::~DirectDeclarator() {
    delete declarator;
    delete array;
}

Nodes DirectDeclarator::nodes() const {
    Nodes nodes;

    if(identifier) nodes.push_back(identifier);
    else nodes.push_back(declarator);

    if(array) nodes.push_back(array);
    return nodes;
}

Node* DirectDeclarator::construct() {
    Token* token = nullptr;
    Node* node = nullptr;
    Node* array = nullptr;

    if(parser->next().has({"("})) {
        parser->take();
        node = Declarator::construct();
        parser->expectingHas({")"});
    } else token = &parser->expectingOf({Token::IDENTIFIER});
    
    if(parser->next().has({"["})) array = ArrayDeclarator::construct();
    return new DirectDeclarator(token, node, array);
}

Transpiler::Line DirectDeclarator::transpile() {
    Transpiler::Line line;

    if(identifier) line.replace(identifier->string);
    else line = declarator->transpile().add("(", ")");

    if(array) line.add("", " " + array->transpile().toString());
    return line;
}
