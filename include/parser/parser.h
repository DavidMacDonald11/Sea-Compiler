#ifndef PARSER_H
#define PARSER_H

#include "lexer/token.h"

class Node;

class Parser {
    vector<Token>& tokens;
public:
    nat i;
    Node* tree;

    Parser(vector<Token>& tokens);
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
