#include "parser/statements/include-statement-component.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"

IncludeStatementComponent::IncludeStatementComponent(Token& token)
: PrimaryNode(token) {}

Node* IncludeStatementComponent::construct() {
    if(not parser->next().has({"include"})) return nullptr;

    parser->take();
    Token& token = parser->expectingOf({Token::STR});

    return new IncludeStatementComponent(token);
}

Transpiler::Line IncludeStatementComponent::transpile() {
    Transpiler::Line line = {"", token.string};
    return line.add("#include ").finish(self, false);
}
