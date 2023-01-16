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

Transpiler::Line FileStatement::transpile(Transpiler& transpiler) {
    for(Node* statement : statements) {
        transpiler.write(statement->transpile(transpiler).toString(), "");
    }

    return {};
}

void FileStatement::makeTree(Parser& parser) {
    while(not parser.next().has({""})) {
        Node* node = Statement::construct(parser);
        if(not node) return;
        statements.push_back(node);
    }
}
