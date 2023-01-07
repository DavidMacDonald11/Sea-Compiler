#include "parser/statements/file-statement.h"
#include "parser/statements/statement.h"
#include "transpiler/transpiler.h"
#include "util.h"

vector<Component*> FileStatement::nodes() const { 
    vector<Component*> nodes;
    nodes.reserve(statements.size());
    
    for(Node* node : statements) nodes.push_back(node);
    return nodes;
}

FileStatement::~FileStatement() {
    for(Component* ptr : statements) delete ptr;
}

Transpiler::Line FileStatement::transpile() {
    for(Node* statement : statements) {
        transpiler->write(statement->transpile().toString(), "");
    }

    return {};
}

void FileStatement::makeTree() {
    while(not parser->next().has({""})) 
        statements.push_back(Statement::construct());
}
