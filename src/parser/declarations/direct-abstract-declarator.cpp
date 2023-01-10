#include "parser/declarations/direct-abstract-declarator.h"
#include "parser/declarations/abstract-declarator.h"
#include "parser/declarations/array-declarator.h"
#include "transpiler/transpiler.h"

DirectAbstractDeclarator::DirectAbstractDeclarator(Node& declarator, Node* array)
: declarator(declarator), array(array) {}

DirectAbstractDeclarator::~DirectAbstractDeclarator() {
    delete &declarator;
    delete array;
}

Nodes DirectAbstractDeclarator::nodes() const {
    Nodes nodes = {&declarator};
    if(array) nodes.push_back(array);
    return nodes;
}

Node* DirectAbstractDeclarator::construct() {
    if(not parser->next().has({"("})) return nullptr;

    parser->take();
    Node* declarator = AbstractDeclarator::construct();
    parser->expectingHas({")"});

    Node* array = (parser->next().has({"["}))? ArrayDeclarator::construct() : nullptr;
    return new DirectAbstractDeclarator(*declarator, array);
}

Transpiler::Line DirectAbstractDeclarator::transpile() {
    Transpiler::Line line;
    
    line.add("(", declarator.transpile().toString());
    if(array) line.add("", array->transpile().toString());

    return line.add("", ")");
}
