#ifndef NODE_H
#define NODE_H

#include "lexer/source-line.h"
#include "transpiler/transpiler.h"
#include "component.h"
#include "parser.h"

struct Node : public Component {
    static Parser* parser;
    static Transpiler* transpiler;

    virtual vector<Component*> nodes() const = 0;
    virtual str toString() const override;
    virtual str tree(str prefix) const override;
    virtual vector<SourceLine*> lines() const override; 
    virtual str raw() const override;
    virtual void mark() override;

    //static virtual Node* construct();
    virtual Transpiler::Line transpile() { return {}; };
};

struct PrimaryNode : public Node {
    Token& token;

    virtual vector<Component*> nodes() const override;
    virtual str tree(str) const override;

    PrimaryNode(Token& token);
};

struct BinaryOperation : public Node {
    Node& left;
    Token& op;
    Node& right;

    vector<Component*> nodes() const override;

    BinaryOperation(Node& left, Token& op, Node& right);
    ~BinaryOperation();

    template <class Fun, class Func>
    static Node* construct(vector<str> hasList, Fun makeChild, Func make) {
        Node* node = makeChild();

        while(parser->next().has(hasList)) {
            Token& op = parser->take();
            Node& right = *makeChild();
            node = make(*node, op, right);
        }

        return node;
    }

    virtual Transpiler::Line transpile() override;
    virtual Transpiler::Line transpileBinary(str op);
};

#endif //NODE_H
