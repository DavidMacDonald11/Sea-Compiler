#ifndef PARSER_H
#define PARSER_H

#include "fault.h"
#include "lexer/token.h"

class Node;

class Parser {
    vector<Token>& tokens;
public:
    struct Context {
        bool mustEndLineStatement = true;
        bool allowNullStatements = true;
        bool allowDirectAbstractDeclarator = false;
    };

    Fault& fault;
    nat i;
    Node* tree;
    Context context;

    Parser(Fault& fault, vector<Token>& tokens);
    ~Parser();

    str toString() const;
    void makeTree();
    Token& next() const;
    Token& ahead(nat pos) const;
    Token& take();
    bool nextOrAfterHas(vector<str> values);
    Token& expectingOf(vector<Token::Type> types);
    Token& expectingHas(vector<str> values);
    void skipNewlines();
};

#endif //PARSER_H
