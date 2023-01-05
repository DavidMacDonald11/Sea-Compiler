#ifndef NODE_H
#define NODE_H

#include "lexer/source_line.h"
#include "util/component.h"
#include "parser.h"

class Node : public Component {
public:
    static Parser* parser;

    virtual vector<Component*> nodes() const = 0;
    virtual str toString() const override;
    virtual str tree(str prefix) const override;
    virtual vector<SourceLine*> lines() const override; 
    virtual str raw() const override;
    virtual void mark() override;
};

class PrimaryNode : public Node {
public:
    Token& token;

    virtual vector<Component*> nodes() const override;
    virtual str tree(str) const override;

    PrimaryNode(Token& token);
};

#endif //NODE_H
