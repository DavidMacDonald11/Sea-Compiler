#include "parser/declarations/atomic-type-specifier.h"
#include "parser/declarations/type-name.h"
#include "transpiler/transpiler.h"

AtomicTypeSpecifier::AtomicTypeSpecifier(Node& type)
: type(type) {}

AtomicTypeSpecifier::~AtomicTypeSpecifier() {
    delete &type;
}

Nodes AtomicTypeSpecifier::nodes() const {
    return {&type};
}

Node* AtomicTypeSpecifier::construct() {
    parser->expectingHas({"atomic"});
    parser->expectingHas({"("});
    Node* type = TypeName::construct();
    parser->expectingHas({")"});

    return new AtomicTypeSpecifier(*type);
}

Transpiler::Line AtomicTypeSpecifier::transpile() {
    return type.transpile().add("_Atomic(", ")");
}
