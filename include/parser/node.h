#ifndef NODE_H
#define NODE_H

#include "lexer/source-line.h"
#include "transpiler/transpiler.h"
#include "component.h"
#include "parser.h"

using Nodes = vector<Component*>;

struct Node : public Component {
    virtual Nodes nodes() const = 0;
    virtual str toString() const override;
    virtual str tree(str prefix) const override;
    virtual vector<SourceLine*> lines() const override; 
    virtual str raw() const override;
    virtual void mark() override;

    //static virtual Node* construct(Parser& parser);
    virtual Transpiler::Line transpile(Transpiler&) { return {}; };
};

struct PrimaryNode : public Node {
    Token& token;

    virtual Nodes nodes() const override;
    virtual str tree(str) const override;

    PrimaryNode(Token& token);
};

struct BinaryOperation : public Node {
    Node& left;
    Token& op;
    Node& right;

    Nodes nodes() const override;

    BinaryOperation(Node& left, Token& op, Node& right);
    ~BinaryOperation();

    template <class Fun, class Func>
    static Node* construct(Parser& parser, vector<str> hasList, Fun makeChild, Func make) {
        Node* node = makeChild(parser);

        while(parser.next().has(hasList)) {
            Token& op = parser.take();
            Node& right = *makeChild(parser);
            node = make(*node, op, right);
        }

        return node;
    }

    virtual Transpiler::Line transpile(Transpiler& transpiler) override;
    virtual Transpiler::Line transpileBinary(Transpiler& transpiler, str op);
};

#endif //NODE_H
