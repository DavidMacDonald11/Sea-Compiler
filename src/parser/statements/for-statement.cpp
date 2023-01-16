#include "parser/statements/for-statement.h"
#include "parser/expressions/expression.h"
#include "parser/statements/line-statement-component.h"
#include "parser/statements/statement.h"

ForStatement::ForStatement(Token* label, Node& declaration, Node& condition, Node& mod, Node& statement)
: label(label), declaration(declaration), condition(condition), mod(mod), statement(statement) {}

ForStatement::~ForStatement() {
    delete &declaration;
    delete &condition;
    delete &mod;
}

Nodes ForStatement::nodes() const {
    return label? Nodes{label, &declaration, &condition, &mod, &statement} 
        : Nodes{&declaration, &condition, &mod, &statement};
}

Node* ForStatement::construct(Parser& parser) {
    if(not parser.nextOrAfterHas({"for"})) return nullptr;

    Token* label = parser.next().of({Token::IDENTIFIER})? &parser.take() : nullptr;
    parser.expectingHas({"for"});

    Token* paren = nullptr;

    if(parser.next().has({"("})) {
        paren = &parser.take();
        parser.skipNewlines();
    }

    Node& declaration = *LineStatementComponent::construct(parser);
    takeEndOrKeyword(parser, "while");

    Node& condition = *Expression::construct(parser);
    takeEndOrKeyword(parser, "then");

    Node& modification = *LineStatementComponent::construct(parser);
    if(paren) parser.expectingHas({")"});
    parser.skipNewlines();

    if(parser.next().has({"do"})) parser.take();
    parser.skipNewlines();

    Node& statement = *Statement::construct(parser);
    return new ForStatement(label, declaration, condition, modification, statement);
}

void ForStatement::takeEndOrKeyword(Parser& parser, str keyword) {
    Token* end = (parser.next().has(Token::LINE_ENDS))? &parser.take() : nullptr;
    parser.skipNewlines();

    if(end) {
        if(parser.next().has({keyword})) parser.take();
        return;
    }

    parser.expectingHas({keyword});
}
