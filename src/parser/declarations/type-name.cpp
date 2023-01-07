#include "parser/declarations/type-name.h"
#include "parser/declarations/specifier-qualifier-list.h"

vector<Component*> TypeName::nodes() const {
    return {&list};
}

TypeName::TypeName(Node& list)
: list(list) {}

TypeName::~TypeName() {
    delete &list;
}

Node* TypeName::construct() {
    Node* list = SpecifierQualifierList::construct();
    return new TypeName(*list);
}

Transpiler::Line TypeName::transpile() {
    return list.transpile();
}
