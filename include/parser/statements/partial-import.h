#ifndef PARTIAL_IMPORT_H
#define PARTIAL_IMPORT_H

#include "../node.h"

struct PartialImport : public Node {
    Token& import;
    Token* rename;

    PartialImport(Token& import, Token* rename = nullptr);

    Nodes nodes() const override;
    static Node* construct();
};

#endif //PARTIAL_IMPORT_H
