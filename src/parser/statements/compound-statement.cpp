#include "parser/statements/compound-statement.h"
#include "parser/statements/statement.h"
#include "transpiler/transpiler.h"

vector<Component*> CompoundStatement::nodes() const {
    vector<Component*> nodes;
    nodes.reserve(statements.size());

    for(Node* node : statements) nodes.push_back(node);
    return nodes;
}

CompoundStatement::CompoundStatement(vector<Node*> statements) 
: statements(statements) {}

CompoundStatement::~CompoundStatement() {
    for(Node* node : statements) delete node;
}

Node* CompoundStatement::construct() {
    if(not parser->next().has({"{"})) return nullptr;

    parser->take();

    vector<Node*> statements;

    while(not parser->next().has({"}", ""})) {
        parser->skipNewlines();
        if(parser->next().has({"}", ""})) break;
        statements.push_back(Statement::construct());
    }
    
    parser->expectingHas({"}"});
    parser->skipNewlines();
    
    return new CompoundStatement(statements);
}

Transpiler::Line CompoundStatement::transpile() {
    Transpiler::Line start("", "{");
    Transpiler::Line line("", "}");

    start.finish(self, false);
    line.prefix(start).finish(self);

    transpiler->context.indent += 1;
    for(Node* node : statements) line.prefix(node->transpile());
    transpiler->context.indent -= 1;

    return line;
}
