#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include "source_file.h"
#include "../util.h"

class Lexer {
public:
    vector<Token> tokens;
    SourceFile& file;

    Lexer(SourceFile& file);

    str toString() const;
    Token newToken(Token::Type type);

    void makeTokens();
    void makeToken();
    void ignoreSpaces();
    void makeOperator();
};

#endif
