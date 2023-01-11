#ifndef IMPORT_STATEMENT_COMPONENT_H
#define IMPORT_STATEMENT_COMPONENT_H

#include "../node.h"

struct ImportStatementComponent : public Node {
    Token& file;
    Token* rename;

    ImportStatementComponent(Token& file, Token* rename = nullptr);

    Nodes nodes() const override;
    static Node* construct();
};

struct FromImportStatementComponent : public Node {
    Token& file;
    vector<Node*> imports;
    Token* all;

    FromImportStatementComponent(Token& file, vector<Node*> imports);
    FromImportStatementComponent(Token& file, Token* all);
    ~FromImportStatementComponent();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //IMPORT_STATEMENT_COMPONENT_H
