#ifndef ENUM_DEFINITION_H
#define ENUM_DEFINITION_H

#include "../node.h"

struct EnumDefinition : public Node {
    Token* visibility;
    Token& identifier;
    vector<Node*> enumerators;

    EnumDefinition(Token* visibility, Token& identifier, vector<Node*> enumerators);
    ~EnumDefinition();

    Nodes nodes() const override;
    static Node* construct();
};

#endif //ENUM_DEFINITION_H
