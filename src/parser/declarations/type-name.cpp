#include "parser/declarations/type-name.h"
#include "parser/declarations/specifier-qualifier-list.h"
#include "parser/declarations/abstract-declarator.h"
#include "transpiler/transpiler.h"

TypeName::TypeName(Node& list, Node* declarator)
: list(list), declarator(declarator) {}

TypeName::~TypeName() {
    delete &list;
    delete declarator;
}

Nodes TypeName::nodes() const {
    Nodes nodes = {&list};
    if(declarator) nodes.push_back(declarator);
    return nodes;
}

Node* TypeName::construct(Parser& parser) {
    Node* list = SpecifierQualifierList::construct(parser);
    return new TypeName(*list, AbstractDeclarator::construct(parser));
}

Transpiler::Line TypeName::transpile(Transpiler& transpiler) {
    Transpiler::Line line = list.transpile(transpiler);

    if(declarator) {
        Transpiler::Line declarator = self.declarator->transpile(transpiler);
        line.add("", declarator.toString());
        line.pointers = declarator.pointers;
    }

    return line;
}
