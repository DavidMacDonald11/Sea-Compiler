#ifndef STRUCT_DEFINITION_H
#define STRUCT_DEFINITION_H

#include "../node.h"

struct StructDefinition : public Node {
    Token* visibility;
    Token& keyword;
    Token& name;
    vector<Node*> list;

    StructDefinition(Token* visibility, Token& keyword, Token& name, vector<Node*> list);
    ~StructDefinition();

    Nodes nodes() const override;
    static Node* construct(Parser& parser);
};

#endif //STRUCT_DEFINITION_H
