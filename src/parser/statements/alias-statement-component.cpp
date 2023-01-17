#include "parser/statements/alias-statement-component.h"
#include "parser/declarations/type-name.h"
#include "parser/node.h"
#include "publisher/publisher.h"
#include "transpiler/transpiler.h"

AliasStatementComponent::AliasStatementComponent(Token* visibility, Node& type, Token& identifier)
: visibility(visibility), type(type), identifier(identifier) {}

AliasStatementComponent::~AliasStatementComponent() {
    delete &type;
}

Nodes AliasStatementComponent::nodes() const {
    Nodes nodes;

    if(visibility) nodes.push_back(visibility);
    nodes.push_back(&type);
    nodes.push_back(&identifier);

    return nodes;
}

Node* AliasStatementComponent::construct(Parser& parser) {
    if(not parser.nextOrAfterHas({"alias"})) return nullptr;
    
    Token* visibility = nullptr;
    if(parser.next().has({Token::VISIBILITY_KEYWORDS})) visibility = &parser.take();

    parser.expectingHas({"alias"});
    Node* type = TypeName::construct(parser);
    parser.expectingHas({"as"});
    Token& identifier = parser.expectingOf({Token::IDENTIFIER});

    return new AliasStatementComponent(visibility, *type, identifier);
}

Publisher::Value* AliasStatementComponent::publish(Publisher &) {
    return nullptr;
}

Transpiler::Line AliasStatementComponent::transpile(Transpiler& transpiler) {
    Transpiler::Line line = type.transpile(transpiler);
    return line.add("typedef ", " " + identifier.string);
}
