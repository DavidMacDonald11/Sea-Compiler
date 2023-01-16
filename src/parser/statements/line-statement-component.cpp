#include "parser/statements/line-statement-component.h"
#include "parser/declarations/assert-declaration.h"
#include "parser/declarations/declaration.h"
#include "parser/expressions/expression.h"
#include "parser/statements/alias-statement-component.h"
#include "parser/statements/basic-statement-component.h"
#include "parser/statements/hidden-statement.h"
#include "parser/statements/import-statement-component.h"
#include "parser/statements/include-statement-component.h"
#include "parser/statements/reassign-statement-component.h"
#include "transpiler/transpiler.h"

LineStatementComponent::LineStatementComponent(Node& statement)
: HiddenStatement(statement) {}

Node* LineStatementComponent::construct(Parser& parser) {
    Node* node;
    Token& next = parser.next();

    node = AliasStatementComponent::construct(parser);
    node = node? node : IncludeStatementComponent::construct(parser);
    node = node? node : BasicStatementComponent::construct(parser);

    if(node) return new LineStatementComponent(*node);

    if(next.has({"assert"})) node = AssertDeclaration::construct(parser);
    else if(next.has(Token::DECLARATION_KEYWORDS)) node = Declaration::construct(parser);
    else if(next.has({"from", "import"})) 
        node = ImportStatementComponent::construct(parser);
    else {
        node = Expression::construct(parser);

        if(parser.next().has(Token::ASSIGN_OPS)) 
            node = ReassignStatementComponent::construct(parser, *node);
    }

    if(in(className(node), vector<str>{"Identifier", "FileIdentifier"
        }) and not parser.next().has(Token::LINE_ENDS)) {
        parser.i -= (className(node) == "Identifier")? 1 : 3;
        node = Declaration::construct(parser);
    } 
    
    return new LineStatementComponent(*node);
}

Transpiler::Line LineStatementComponent::transpile(Transpiler& transpiler) {
    Transpiler::Line line = statement.transpile(transpiler); 

    if(className(&statement) != "AssertDeclaration") {
        return line.setShowType();
    }

    return line;
}
