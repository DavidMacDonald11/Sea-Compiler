#include "parser/statements/hidden-statement.h"

vector<Component*> HiddenStatement::nodes() const { return {&statement}; }
str HiddenStatement::tree(str prefix) const { return statement.tree(prefix); }

HiddenStatement::HiddenStatement(Node& statement)
: statement(statement) {}

HiddenStatement::~HiddenStatement() {
    delete &statement;
}

Transpiler::Line HiddenStatement::transpile() { return statement.transpile(); }
