#ifndef SUBSTITUTOR_H
#define SUBSTITUTOR_H

#include "lexer/token.h"

class Substitutor {
    vector<Token>& lexerTokens;
public:
    vector<Token> tokens;
    nat i;

    Substitutor(vector<Token>& tokens);
    
    str toString() const;
    Token& next() const;
    Token& ahead(nat pos) const;
    Token& take();
    void skip();
    void add(Token token);
    Token combineSign(Token* sign, Token token);

    void replaceTokens();
    void checkToken();
    void replaceRange();
    void replaceIntRange(Token left, Token right, long long* step = nullptr);
    void replaceCharRange(Token left, Token right, long long* step = nullptr);
};

#endif //SUBSTITUTOR_H
