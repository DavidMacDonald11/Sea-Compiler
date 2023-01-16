#include "parser/statements/include-statement-component.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"
#include "util.h"

IncludeStatementComponent::IncludeStatementComponent(Token* lib, Token& token)
: lib(lib), token(token) {}

Nodes IncludeStatementComponent::nodes() const {
    if(lib) return {lib, &token};
    return {&token};
}

Node* IncludeStatementComponent::construct() {
    if(not parser->next().has({"include"})) return nullptr;
    parser->take();

    Token* lib = (parser->next().has({"lib"}))? &parser->take() : nullptr;
    Token& token = parser->expectingOf({Token::STR});

    return new IncludeStatementComponent(lib, token);
}

Transpiler::Line IncludeStatementComponent::transpile() {
    str string = (lib)? "<" + replaceStr(token.string, "\"", "") + ">" : token.string;
    Transpiler::Line line = {"", string};
    return line.add("#include ").finish(self, false);
}
