#include "parser/statements/line-statement-component.h"
#include "parser/declarations/assert-declaration.h"
#include "parser/declarations/declaration.h"
#include "parser/expressions/expression.h"
#include "parser/statements/hidden-statement.h"
#include "transpiler/transpiler.h"

LineStatementComponent::LineStatementComponent(Node& statement)
: HiddenStatement(statement) {}

Node* LineStatementComponent::construct() {
    Node* node;

    if(parser->next().has({"assert"})) node = AssertDeclaration::construct();
    else if(parser->next().has({"static"})) node = Declaration::construct();
    else node = Expression::construct();

    return new LineStatementComponent(*node);
}

Transpiler::Line LineStatementComponent::transpile() {
    Transpiler::Line line = statement.transpile(); 

    if(className(&statement) != "AssertDeclaration") {
        return line.setShowType();
    }

    return line;
}
