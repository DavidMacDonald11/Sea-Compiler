#include "parser/statements/alias-statement.h"
#include "parser/declarations/type-name.h"
#include "parser/node.h"
#include "transpiler/transpiler.h"

AliasStatement::AliasStatement(Token* visibility, Node& type, Token& identifier)
: visibility(visibility), type(type), identifier(identifier) {}

AliasStatement::~AliasStatement() {
    delete &type;
}

Nodes AliasStatement::nodes() const {
    Nodes nodes;

    if(visibility) nodes.push_back(visibility);
    nodes.push_back(&type);
    nodes.push_back(&identifier);

    return nodes;
}

Node* AliasStatement::construct() {
    Token* visibility = nullptr;
    if(parser->next().has({Token::VISIBILITY_KEYWORDS})) visibility = &parser->take();

    if(not parser->next().has({"alias"})) {
        parser->i -= (visibility)? 1 : 0;
        return nullptr;
    }

    parser->take();
    Node* type = TypeName::construct();
    parser->expectingHas({"as"});
    Token& identifier = parser->expectingOf({Token::IDENTIFIER});
    parser->expectingHas(Token::LINE_ENDS);

    return new AliasStatement(visibility, *type, identifier);
}

Transpiler::Line AliasStatement::transpile() {
    Transpiler::Line line = type.transpile();
    line.add("typedef ", " " + identifier.string);
    return line.finish(self, true);
}
