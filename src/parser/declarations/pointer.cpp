#include "parser/declarations/pointer.h"
#include "parser/declarations/type-qualifier-list.h"
#include "transpiler/transpiler.h"

Pointer::Pointer(Token& token, Node* qualifiers, Node* pointer) 
: token(token), qualifiers(qualifiers), pointer(pointer) {}

Pointer::~Pointer() {
    delete qualifiers;
    delete pointer;
}

Nodes Pointer::nodes() const {
    Nodes nodes = {&token};
    if(qualifiers) nodes.push_back(qualifiers);
    return nodes;
}

Node* Pointer::construct(Parser& parser) {
    if(not parser.next().has({"^"})) return nullptr;

    Token& token = parser.take();
    Node* qualifiers = nullptr;

    if(parser.next().has(Token::TYPE_QUALIFIER_KEYWORDS)) 
        qualifiers = TypeQualifierList::construct(parser);

    return new Pointer(token, qualifiers, Pointer::construct(parser));
}

Transpiler::Line Pointer::transpile(Transpiler& transpiler) {
    Transpiler::Line line = {"", "*"};
    line.pointers = 1;
    
    if(qualifiers) line.add("", qualifiers->transpile(transpiler).toString());

    if(pointer) {
        Transpiler::Line ptr = pointer->transpile(transpiler);
        line.pointers += ptr.pointers;
        line.add("", ptr.toString());
    }

    return line;
}
