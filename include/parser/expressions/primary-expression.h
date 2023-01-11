#ifndef PRIMARY_EXPRESSION_H
#define PRIMARY_EXPRESSION_H

#include "../node.h"
#include "parser/component.h"
#include "transpiler/transpiler.h"

struct PrimaryExpression : public Node {
    static Node* construct();
};

struct NumericConstant : public PrimaryNode {
    NumericConstant(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct CharacterConstant : public PrimaryNode {
    CharacterConstant(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct StringConstant : public PrimaryNode {
    StringConstant(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct FileIdentifier : public Node {
    Token& file;
    Token& identifier;

    vector<Component*> nodes() const override;

    FileIdentifier(Token& file, Token& identifier);

    static Node* construct();
    Transpiler::Line transpile() override;
};

struct Identifier : public PrimaryNode {
    Identifier(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct ParentheseseExpression : public Node {
    Node& expression;

    vector<Component*> nodes() const override;

    ParentheseseExpression(Node& expression);
    ~ParentheseseExpression();

    static Node* construct();
    Transpiler::Line transpile() override;
};

struct InitializerListExpression : public Node {
    Node& initializerList;

    InitializerListExpression(Node& initializerList);
    ~InitializerListExpression();

    Nodes nodes() const override;
    static Node* construct();
    Transpiler::Line transpile() override;
};

struct PrimaryKeyword : public PrimaryNode {
    PrimaryKeyword(Token& token);
    static Node* construct();
    Transpiler::Line transpile() override;
};

#endif //PRIMARY_EXPRESSION_H
