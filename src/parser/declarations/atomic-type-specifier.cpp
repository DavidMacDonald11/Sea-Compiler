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

Node* AtomicTypeSpecifier::construct(Parser& parser) {
    parser.expectingHas({"atomic"});
    parser.expectingHas({"("});
    Node* type = TypeName::construct(parser);
    parser.expectingHas({")"});

    return new AtomicTypeSpecifier(*type);
}

Transpiler::Line AtomicTypeSpecifier::transpile(Transpiler& transpiler) {
    return type.transpile(transpiler).add("_Atomic(", ")");
}
