#include "parser/statements/file-statement.h"
#include "parser/statements/statement.h"
#include "util.h"
#include <bits/types/FILE.h>

vector<Component*> FileStatement::nodes() const { return statements; }

FileStatement::~FileStatement() {
    for(Component* ptr : statements) delete ptr;
}

void FileStatement::makeTree() {
    while(not parser->next().has({""})) 
        statements.push_back(Statement::construct());
}
