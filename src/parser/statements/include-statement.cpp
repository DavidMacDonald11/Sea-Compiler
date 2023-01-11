#include "parser/statements/include-statement.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"

IncludeStatement::IncludeStatement(Token& token)
: PrimaryNode(token) {}

Node* IncludeStatement::construct() {
    if(not parser->next().has({"include"})) return nullptr;

    parser->take();
    Token& token = parser->expectingOf({Token::STR});
    parser->expectingHas(Token::LINE_ENDS);

    return new IncludeStatement(token);
}

Transpiler::Line IncludeStatement::transpile() {
    Transpiler::Line line = {"", token.string};
    return line.add("#include ").finish(self, false);
}
