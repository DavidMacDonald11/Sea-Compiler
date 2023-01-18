#include "parser/declarations/pointers.h"
#include "parser/declarations/array-style-pointer.h"
#include "parser/declarations/type-qualifier-list.h"
#include "publisher/publisher.h"
#include "transpiler/transpiler.h"

Pointers::Pointers(Node* qualifiers, Token& token, Node* pointer) 
: qualifiers(qualifiers), token(token), pointer(pointer) {}

Pointers::~Pointers() {
    delete qualifiers;
    delete pointer;
}

Nodes Pointers::nodes() const {
    Nodes nodes;

    if(qualifiers) nodes.push_back(qualifiers);
    nodes.push_back(&token);
    if(pointer) nodes.push_back(pointer);

    return nodes;
}

Node* Pointers::construct(Parser& parser) {
    if(not parser.next().has(mergeAll<str>({
        {"^", "["}, Token::TYPE_QUALIFIER_KEYWORDS}))) return nullptr;

    Node* qualifiers = nullptr;

    if(parser.next().has(Token::TYPE_QUALIFIER_KEYWORDS)) 
        qualifiers = TypeQualifierList::construct(parser);

    if(parser.next().has({"["})) 
        return ArrayStylePointer::construct(parser, qualifiers);

    Token& token = parser.take();
    return new Pointers(qualifiers, token, Pointers::construct(parser));
}

Transpiler::Line Pointers::transpile(Transpiler& transpiler) {
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
