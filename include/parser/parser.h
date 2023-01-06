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

    void makeTree();
    Token& next() const;
    Token& take();
    Token& expectingOf(vector<Token::Type> types);
    Token& expectingHas(vector<str> values);
};

#endif //PARSER_H
