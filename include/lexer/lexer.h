#ifndef LEXER_H
#define LEXER_H

#include "lexer/source_line.h"
#include "token.h"
#include "source_file.h"
#include "../util.h"

class Lexer {
public:
    vector<Token> tokens;
    SourceFile& file;

    Lexer(SourceFile& file);

    str toString() const;
    Token newToken(Token::Type type, SourceLine& line);
    Token newToken(Token::Type type);

    void makeTokens();
    void makeToken();
    void ignoreSpaces();
    void makeNewline();
    void makeOperator();
    void makeNumber();
    void makeIdentifier();
};

#endif
