#include "parser/declarations/direct-abstract-declarator.h"
#include "parser/declarations/abstract-declarator.h"
#include "parser/declarations/array-declarator.h"
#include "publisher/publisher.h"
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

Node* DirectAbstractDeclarator::construct(Parser& parser) {
    if(not parser.next().has({"("})) return nullptr;

    parser.take();
    Node* declarator = AbstractDeclarator::construct(parser);
    parser.expectingHas({")"});

    Node* array = (parser.next().has({"["}))? ArrayDeclarator::construct(parser) : nullptr;
    return new DirectAbstractDeclarator(*declarator, array);
}

Publisher::Value* DirectAbstractDeclarator::publish(Publisher &publisher) {
    Publisher::Declarator* decl = static_cast<Publisher::Declarator*>(declarator.publish(publisher));
    if(not array) return decl;

    Publisher::Declarator* last = decl;
    while(last->declarator != nullptr) last = decl->declarator;
    last->declarator = static_cast<Publisher::Declarator*>(array->publish(publisher));
    
    return decl;
}

Transpiler::Line DirectAbstractDeclarator::transpile(Transpiler& transpiler) {
    Transpiler::Line line;
    
    line.add("(", declarator.transpile(transpiler).toString());
    if(array) line.add("", array->transpile(transpiler).toString());

    return line.add("", ")");
}
